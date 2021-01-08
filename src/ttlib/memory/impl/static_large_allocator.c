/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       static_large_allocator.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-12-25 
 * @brief      static_large_allocator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "static_large_allocator.h"
#include "../../platform/native_memcpy.h"
#include "../../libm/libm.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// get base of head_data
#define tt_static_large_data_allocator_base(head_data)  (&(((tt_pool_data_head_t*)((tt_static_large_data_head_t*)(data_head) + 1))[-1]))  

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the static large data head type
typedef __tt_aligned__(TT_CPU_BITBYTE) struct __tt_static_large_data_head_t 
{
    // the data space
    tt_size_t   space : 31;

    // the data be free?
    tt_size_t   bfree : 1 ;

    // the data head base
    tt_byte_t   base[sizeof(tt_pool_data_head_t)];

}__tt_aligned__(TT_CPU_BITBYTE) tt_static_large_data_head_t, *tt_static_large_data_head_ref_t;

// the static large data pred type
typedef struct __tt_static_large_data_pred_t
{
    // the static large data head
    tt_static_large_data_head_t*  data_head;

}tt_static_large_data_pred_t, *tt_static_large_data_pred_ref_t;

/*! the static large allocator type
 *
 * <pre>
 *
 * .e.g page_size == 4KB
 *
 *       +--------------------------------------------------------------------------+
 *       |                                     data                                 |
 *       +--------------------------------------------------------------------------+
 *                                              |
 *       +--------------------------------------------------------------------------+
 *       | head | 4KB | 16KB | 8KB | 128KB | ... | 32KB |       ...       |  4KB*N  |
 *       +--------------------------------------------------------------------------+
 *                       |                       |               |
 *                       |                       `---------------`
 *                       |                        merge free space when alloc or free
 *                       |
 *       +------------------------------------------+
 *       | tb_static_large_data_head_t | data space |
 *       +------------------------------------------+
 *                       |
 *                       |
 *       +----------------------+
 *       | space | bfree | base | 
 *       +----------------------+
 * 
 * 
 *       +--------------------------------------+
 * pred: | >0KB :      4KB       | > 0*page     | 1
 *       |-----------------------|--------------+
 *       | >4KB :      8KB       | > 1*page     | 2
 *       |-----------------------|--------------+
 *       | >8KB :    12-16KB     | > 2*page     | 3-4
 *       |-----------------------|--------------+
 *       | >16KB :   20-32KB     | > 4*page     | 5-8
 *       |-----------------------|--------------+
 *       | >32KB :   36-64KB     | > 8*page     | 9-16
 *       |-----------------------|--------------+
 *       | >64KB :   68-128KB    | > 16*page    | 17-32
 *       |-----------------------|--------------+
 *       | >128KB :  132-256KB   | > 32*page    | 33-64
 *       |-----------------------|--------------+
 *       | >256KB :  260-512KB   | > 64*page    | 65 - 128
 *       |-----------------------|--------------+
 *       | >512KB :  516-1024KB  | > 128*page   | 129 - 256
 *       |-----------------------|--------------+
 *       | >1024KB : 1028-...KB  | > 256*page   | 257 - ..
 *       +--------------------------------------+
 *
 * </pre>
 */

// the static large allocator type
typedef __tt_aligned__(TT_CPU_BITBYTE) struct __tt_static_large_allocator_t
{
    // base
    tt_allocator_t                 base;

    // the page size
    tt_size_t                      page_size;

    // the data total size
    tt_size_t                      data_size;

    // the data head
    tt_static_large_data_head_t*   data_head; 

    // the data tail
    tt_static_large_data_head_t*   data_tail; 

    // the data pred
    tt_static_large_data_pred_t    pred[10];

}__tt_aligned__(TT_CPU_BITBYTE) tt_static_large_allocator_t, *tt_static_large_allocator_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
static __tt_inline__ tt_void_t tt_static_large_data_allocator_pred_remove(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t data_head);


/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

// page size TODO need opt
static __tt_inline__ tt_size_t tt_page_size(tt_void_t)
{
    return 1024;
}

// find next data_head according to current 
static __tt_inline__ tt_static_large_data_head_ref_t tt_static_large_allocator_next_head(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t current_head)
{
    // check
    tt_check_return_val(allocator && current_head, tt_null);

    // done
    tt_static_large_data_head_ref_t next_head;
    tt_bool_t                       ok = 0;
    do
    {
        // find next head
        next_head = (tt_static_large_data_head_ref_t)((tt_byte_t* )&current_head[1] + current_head->space); 

        // is tail?
        tt_check_break(next_head < allocator->data_tail);

        // ok
        ok = 1;

    } while (0);

    // not ok?
    if(!ok) next_head = tt_null;
    
    return next_head;
}

// merge next free data until not free
static __tt_inline__ tt_static_large_data_head_ref_t tt_static_large_allocator_merge_data(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t current_head)
{
    // check
    tt_check_return_val(allocator && current_head, tt_null);

    // remove cache
    tt_static_large_data_allocator_pred_remove(allocator, current_head);

    // done
    tt_static_large_data_head_ref_t next_head = tt_null;
    while((next_head = tt_static_large_allocator_next_head(allocator, current_head)) && next_head->bfree)
    {
        // remove next_head from cache
        tt_static_large_data_allocator_pred_remove(allocator, next_head);

        // merge
        current_head->space += sizeof(tt_static_large_data_head_t) + next_head->space;
    }

    return current_head;
}

// find the pred index
static __tt_inline__ tt_size_t tt_static_large_allocator_pred_index(tt_static_large_allocator_ref_t allocator, tt_size_t space)
{
    // check
    tt_assert(allocator);

    // the large data element size, and check algin
    tt_size_t size = space + sizeof(tt_static_large_data_head_t);
    tt_assert(!(size & (allocator->page_size - 1)));

    // computer page cnt
    size /= allocator->page_size;

    // computer index
    tt_size_t index = tt_ilog2i(tt_align_pow2(size));
    if(index >= tt_arrayn(allocator->pred)) index = tt_arrayn(allocator->pred) - 1;

    return index;
}

// update the pred
static __tt_inline__ tt_void_t tt_static_large_data_allocator_pred_update(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t data_head)
{
    // check
    tt_assert_and_check_return(allocator && data_head);
    tt_check_return(data_head < allocator->data_tail);

    // find the index
    tt_size_t index = tt_static_large_allocator_pred_index(allocator, data_head->space);
    tt_static_large_data_head_ref_t pred_head = allocator->pred[index].data_head;

    // update maybe make memory gap 
    if(!pred_head || pred_head->space < data_head->space) allocator->pred[index].data_head = data_head;

}

// remove one pred
static __tt_inline__ tt_void_t tt_static_large_data_allocator_pred_remove(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t data_head)
{
    // check
    tt_assert_and_check_return(allocator && data_head);

    // find the index
    tt_size_t index = tt_static_large_allocator_pred_index(allocator, data_head->space);

    // remove the pred
    if(data_head == allocator->pred[index].data_head) allocator->pred[index].data_head = tt_null;
}

// find the data_head and malloc it
static tt_static_large_data_head_ref_t tt_static_large_data_allocator_malloc_find(tt_static_large_allocator_ref_t allocator, tt_static_large_data_head_ref_t data_head, tt_size_t walk_size, tt_size_t space)
{
    // check 
    tt_assert_and_check_return_val(allocator && data_head && space, tt_null);

    // data tail
    tt_static_large_data_head_ref_t data_tail = allocator->data_tail;
    tt_check_return_val(data_head < data_tail, tt_null); 

    // find the free data
    while(data_head + 1 < data_tail && walk_size)
    {
        // data_space and check
        tt_size_t data_space = data_head->space;
        tt_assert(!((sizeof(tt_static_large_data_head_t) + data_space) & (allocator->page_size -1)));

        // data_head free?
        if(data_head->bfree)
        {
            // space enough?
            if(data_head->space >= space)
            {
                // remove this data_head from pred
                tt_static_large_data_allocator_pred_remove(allocator, data_head);

                // split, if data_head too large
                if(data_head->space > sizeof(tt_static_large_data_head_t) + space)
                {
                    // split
                    tt_static_large_data_head_ref_t data_next = (tt_static_large_data_head_ref_t)((tt_byte_t*)(data_head + 1) + space);
                    data_next->bfree = 1;
                    data_next->space = data_head->space - space - sizeof(tt_static_large_data_head_t);

                    data_head->space = space;

                    // add next data to pred
                    tt_static_large_data_allocator_pred_update(allocator, data_next);
                }
                // data_head->space == space
                else
                {
                    // try cache data_next 
                    tt_static_large_data_head_ref_t data_next = tt_static_large_allocator_next_head(allocator, data_head);
                    if(data_next) tt_static_large_data_allocator_pred_update(allocator, data_next);
                }
                
                // allocatot it
                data_head->bfree = 0;

                // return 
                return data_head;
            }
            // not enough, try to merge next
            else
            {
                tt_static_large_data_head_ref_t data_next = (tt_static_large_data_head_ref_t)((tt_byte_t *)(data_head + 1) + space);

                // if next is tail, break
                tt_check_break(data_next + 1 < allocator->data_tail);

                // try merge next, if free
                if(data_next->bfree)
                {
                    // merge
                    data_head = tt_static_large_allocator_merge_data(allocator, data_head);

                    // cache to pred
                    tt_static_large_data_allocator_pred_update(allocator, data_head);

                    // continue hand this data
                    continue;
                }
            }
        }

        // walk_size--
        walk_size--;

        // not found, move to next
        data_head = tt_static_large_allocator_next_head(allocator, data_head);
    }

    // failure
    return tt_null;
}

// find the pred and malloc it
static tt_static_large_data_head_ref_t tt_static_large_data_allocator_malloc_pred(tt_static_large_allocator_ref_t allocator, tt_size_t space)
{
    // check 
    tt_assert_and_check_return_val(allocator && allocator->data_head, tt_null);

    // walk the pred 
    tt_size_t                          index = tt_static_large_allocator_pred_index(allocator, space);  
    tt_size_t                          size  = tt_arrayn(allocator->pred);
    tt_static_large_data_pred_ref_t    pred = allocator->pred; 
    tt_static_large_data_head_ref_t    data_head = tt_null;
    tt_static_large_data_head_ref_t    pred_head = tt_null;

    for(; index < size && !data_head; index++)
    { 
        // pred head
        pred_head = pred[index].data_head;
        if(pred_head)
        {
            data_head = tt_static_large_data_allocator_malloc_find(allocator, pred_head, 1, space);
        }
    }
    
    // trace
    tt_trace_d("malloc, pred, %lu, %s", space, data_head? "ok": "no");

    // ok?
    return data_head;
}

// malloc done
static tt_static_large_data_head_t* tt_static_large_allocator_malloc_done(tt_static_large_allocator_t* allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    // check 
    tt_assert_and_check_return_val(allocator && allocator->data_head, tt_null);

    // done
    tt_bool_t                      ok = tt_false;
    tt_static_large_data_head_t*   data_head = tt_null;
    do
    {
#ifdef __tt_debug__
        // patch = 0xcc
        tt_size_t patch = 0; // patch = 1
#else
        tt_size_t patch = 0;
#endif

        // compute need space
        tt_size_t need_space = tt_align(size + patch, allocator->page_size) - sizeof(tt_static_large_data_head_t);
        if(need_space < size + patch) need_space = tt_align(size + patch + allocator->page_size, allocator->page_size) - sizeof(tt_static_large_data_head_t);
        tt_assert(need_space > size + patch);

        // try to predict first
        data_head = tt_static_large_data_allocator_malloc_pred(allocator, need_space); 

        // predict failed?
        if(!data_head)
        {
            // find from head
            data_head = tt_static_large_data_allocator_malloc_find(allocator, allocator->data_head, -1, need_space);

            // break, if found failed
            tt_check_break(data_head);
        }

        // the data_head space must > size + patch
        tt_assert(data_head->space > size + patch);

        // the base head
        tt_pool_data_head_t* base_head = tt_static_large_data_allocator_base(data_head);
        
        // the real size
        tt_size_t real_size = real? (data_head->space - patch): size; 
        
        // save the real size
        if(real) *real  = real_size;
        base_head->size = real_size;

        // ok
        ok = tt_true;

    } while (0);

    // trace
    tt_trace_d("malloc, %lu, %s", size, ok? "ok": "failed");
    
    // not ok
    if(!ok) data_head = tt_null;

    // ok
    return data_head;
}

// ralloc fast if next data is free and enough
static tt_static_large_data_head_t* tt_static_large_allocator_ralloc_fast(tt_static_large_allocator_t* allocator,tt_static_large_data_head_t* data_head, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    // check
    tt_assert_and_check_return_val(allocator && allocator->data_head && size, tt_null);

    // done
    tt_bool_t ok = tt_false;
    do
    {
        // data base
        tt_pool_data_head_t* data_base = tt_static_large_data_allocator_base(data_head);

#ifdef __tt_debug__
        // patch = 0xcc
        tt_size_t patch = 0; // patch = 1
#else
        tt_size_t patch = 0;
#endif

        // compute need space
        tt_size_t need_space = tt_align(size + patch, allocator->page_size) - sizeof(tt_static_large_data_head_t);
        if(need_space < size + patch) need_space = tt_align(size + patch + allocator->page_size, allocator->page_size) - sizeof(tt_static_large_data_head_t);
        tt_assert(need_space > size + patch);

        // this data not enough?
        if(need_space > data_head->space)
        {
            // try to merge next
            data_head = tt_static_large_allocator_merge_data(allocator, data_head);
        }

        // not enough, break
        tt_check_break(data_head->space >= need_space);

        // split, if data_head too large
        if (data_head->space > sizeof(tt_static_large_data_head_t) + need_space)
        {
            // split
            tt_static_large_data_head_ref_t data_next = (tt_static_large_data_head_ref_t)((tt_byte_t *)(data_head + 1) + need_space);
            data_next->bfree = 1;
            data_next->space = data_head->space - need_space - sizeof(tt_static_large_data_head_t);

            data_head->space = need_space;

            // add next data to pred
            tt_static_large_data_allocator_pred_update(allocator, data_next);
        }

        // the real size
        tt_size_t real_size = real? (data_head->space - patch): size; 
        
        // save the real size
        if(real) *real  = real_size;
        data_base->size = real_size;

        // ok
        ok = tt_true;

    } while (0);
    
    // trace
    tt_trace_d("ralloc, %lu, %s", size, ok? "ok": "failed");
    
    // not ok
    if(!ok) data_head = tt_null;

    // ok
    return data_head;
}

// free
static tt_bool_t tt_static_large_allocator_free(tt_allocator_ref_t self, tt_pointer_t data __tt_debug_decl__)
{
    // check
    tt_static_large_allocator_t* allocator = (tt_static_large_allocator_t*)self;
    tt_assert_and_check_return_val(allocator && data, tt_false);

    // done
    tt_bool_t                     ok = tt_false;
    tt_static_large_data_head_t*  data_head = tt_null;
    do
    {
        // get data head
        data_head = &((tt_static_large_data_head_t*)data)[-1];

        // check
        tt_assertf_and_check_break(!data_head->bfree, "double free data, %p", data);
        tt_assertf_and_check_break(data_head >= allocator->data_head && data_head < allocator->data_tail, "the data %p not belong the allocator %p", data, allocator);

        // try to merge
        data_head = tt_static_large_allocator_merge_data(allocator, data_head);

        // free 
        data_head->bfree = 1;

        // cache to pred
        tt_static_large_data_allocator_pred_update(allocator, data_head);

        // ok?
        ok = tt_true;
        
    } while (0);
    
    // trace
    tt_trace_d("free, %p, %s", data, ok? "ok": "failed");
    
    return ok;
}

// malloc
static tt_pointer_t tt_static_large_allocator_malloc(tt_allocator_ref_t self, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    // check
    tt_static_large_allocator_t* allocator = (tt_static_large_allocator_t*)self;
    tt_assert_and_check_return_val(allocator && size, tt_null);

    // done
    tt_static_large_data_head_t* data_head = tt_static_large_allocator_malloc_done(allocator, size, real __tt_debug_args__);
    
    // ok?
    return data_head? (tt_pointer_t)&data_head[1]: tt_null;
}

// ralloc
static tt_pointer_t tt_static_large_allocator_ralloc(tt_allocator_ref_t self, tt_pointer_t data, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    // check
    tt_static_large_allocator_t* allocator = (tt_static_large_allocator_t*)self;
    tt_assert_and_check_return_val(allocator && size, tt_null);

    // done
    tt_bool_t                     ok = tt_false;
    tt_byte_t*                    real_data = tt_null;
    tt_static_large_data_head_t*  data_head = tt_null;
    tt_static_large_data_head_t*  aloc_head = tt_null;
    do
    {
        // get data head
        data_head = &(((tt_static_large_data_head_t*)data)[-1]);

        // check
        tt_assertf_and_check_break(!data_head->bfree, "ralloc freed data, %p", data);
        tt_assertf_and_check_break(data_head >= allocator->data_head && data_head < allocator->data_tail, "the data %p not belong the allocator %p", data, allocator);

        // fast ralloc
        aloc_head = tt_static_large_allocator_ralloc_fast(allocator, data_head, size, real __tt_debug_args__);
        
        // failed
        if(!aloc_head)
        {
            // malloc it
            aloc_head = tt_static_large_allocator_malloc_done(allocator, size, real __tt_debug_args__);
            tt_check_break(aloc_head);

            // not the same
            if(aloc_head != data_head)
            {
                // copy 
                tt_memcpy((tt_pointer_t)&aloc_head[1], data, tt_min(size, (((tt_pool_data_head_t*)(data_head + 1))[-1]).size));

                // free previous data
                tt_static_large_allocator_free(self, data __tt_debug_args__);
            }

        }

        // the real data
        real_data = (tt_byte_t *)&aloc_head[1];

        // ok
        ok = tt_true;

    } while (0);

    // trace
    tt_trace_d("ralloc, pred, %lu, %s", size, real_data? "ok": "no");

    // not ok
    if (!ok) real_data = tt_null;

    return (tt_pointer_t)real_data;
}

static tt_void_t tt_static_large_allocator_clear(tt_allocator_ref_t self)
{
    // check
    tt_static_large_allocator_t* allocator = (tt_static_large_allocator_t*)self;
    tt_assert_and_check_return(allocator && allocator->data_head && allocator->data_size > sizeof(tt_static_large_data_head_t));

    // clear head
    allocator->data_head->bfree = 1;
    allocator->data_head->space = allocator->data_size - sizeof(tt_static_large_data_head_t);

    // clear pred
    tt_memset(allocator->pred, 0, sizeof(allocator->pred));

    // updata pred
    tt_static_large_data_allocator_pred_update(allocator, allocator->data_head);

}

static tt_void_t tt_static_large_allocator_exit(tt_allocator_ref_t self)
{
    // check
    tt_static_large_allocator_t* allocator = (tt_static_large_allocator_t*)self;
    tt_assert_and_check_return(allocator);
    
    // exit
    tt_spinlock_exit(&allocator->base.lock);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_allocator_ref_t   tt_static_large_allocator_init(tt_byte_t* data, tt_size_t size, tt_size_t page_size)
{
    // check
    tt_assert_and_check_return_val(data && size, tt_null);

    // check algin
    tt_assert(!(sizeof(tt_static_large_data_head_t) & (TT_CPU_BITBYTE - 1)));
    tt_assert(!(sizeof(tt_static_large_allocator_t) & (TT_CPU_BITBYTE - 1)));

    // algin data and diff
    tt_size_t diff = tt_align((tt_size_t)data, TT_CPU_BITBYTE) - (tt_size_t)data;
    tt_assert_and_check_return_val(size - diff > sizeof(tt_static_large_allocator_t), tt_null);
    size -= diff;
    data += diff; 

    // init allocator
    tt_static_large_allocator_ref_t allocator = (tt_static_large_allocator_t*)data;
    tt_memset(allocator, 0, sizeof(tt_static_large_allocator_t));    

    // init base
    allocator->base.type         = TT_ALLOCATOR_TYPE_LARGE;
    allocator->base.flag         = TT_ALLOCATOR_FLAG_NONE;
    allocator->base.large_malloc = tt_static_large_allocator_malloc;
    allocator->base.large_ralloc = tt_static_large_allocator_ralloc;
    allocator->base.large_free   = tt_static_large_allocator_free;
    allocator->base.clear        = tt_static_large_allocator_clear;
    allocator->base.exit         = tt_static_large_allocator_exit;

    // init lock
    if(!tt_spinlock_init_impl(&allocator->base.lock)) return tt_null;

    // init page size
    allocator->page_size = page_size? page_size: tt_page_size();
    
    // page_size must > head size
    if(allocator->page_size <= sizeof(tt_static_large_data_head_t))
        allocator->page_size += sizeof(tt_static_large_data_head_t);
    
    // page size must be algin
    allocator->page_size = tt_align_pow2(allocator->page_size);
    tt_assert_and_check_return_val(allocator->page_size, tt_null);

    // init data size
    allocator->data_size = size - sizeof(tt_static_large_allocator_t); 
    tt_assert_and_check_return_val(allocator->data_size > allocator->page_size, tt_null);

    // algin data size
    allocator->data_size = tt_align(allocator->data_size - allocator->page_size, allocator->page_size);
    tt_assert_and_check_return_val(allocator->data_size > sizeof(tt_static_large_data_head_t), tt_null);

    // init data head
    allocator->data_head = (tt_static_large_data_head_t *)(&allocator[1]);
    allocator->data_head->bfree = 1;
    allocator->data_head->space = allocator->data_size - sizeof(tt_static_large_data_head_t);
    tt_assert_and_check_return_val(!((tt_size_t)allocator->data_head & (TT_CPU_BITBYTE - 1)), tt_null);

    // init tail
    allocator->data_tail = (tt_static_large_data_head_t*)((tt_byte_t*)&allocator->data_head[1] + allocator->data_head->space);

    // add this free head to pred cache
    tt_static_large_data_allocator_pred_update(allocator, allocator->data_head);

    // ok?
    return (tt_allocator_ref_t)allocator;
}