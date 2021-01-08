/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       static_fixed_pool.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      static_fixed_pool.c file
 */

#define TT_TRACE_MODULE_NAME                 "static_fixed_pool"
#define TT_TRACE_DEBUG                       (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "static_fixed_pool.h"
#include "../../platform/platform.h"

/*//////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#ifdef TT_WORD_BIGENDIAN

#else
//  allocate the index
#   define tt_static_fixed_pool_used_set1(used, i)       do { (used)[i >> 3] |= (0x01 << ((i) & 7)); } while(0)

//  free the index
#   define tt_static_fixed_pool_used_set0(used, i)       do { (used)[i >> 3] &= ~(0x01 << ((i) & 7)); } while(0)

//  the index have been allocated?
#   define tt_static_fixed_pool_used_bset(used, i)       ((used)[(i) >> 3] & (0x01 << ((i) & 7)))

//  find the first bit freed
#   define tt_static_fixed_pool_find_first(v)            tt_bits_fb0_le(~(v))
#endif

//  cached the predicted index
#define tt_static_fixed_pool_cache_pred(pool, i)         do { (pool)->pred_index = ((i) >> TT_CPU_SHIFT) + 1; } while(0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the static fixed pool type
typedef __tt_aligned__(TT_CPU_BITBYTE) struct __tt_static_fixed_pool_t
{
    // the read data addr after the used_info info
    tt_byte_t*             data;

    // the data tail
    tt_byte_t*             tail;

    // the used info
    tt_byte_t*             used_info;

    // the used info size
    tt_size_t              info_size;

    // the predicted index
    tt_size_t              pred_index;

    // the item size
    tt_size_t              item_size;

    // the item space (item head + item size)
    tt_size_t              item_space;

    // the item count
    tt_size_t              item_count;

    // the item maxn
    tt_size_t              item_maxn;

    // the item head size
    tt_uint16_t            data_head_size;

    // for small alloctor?
    tt_uint16_t            for_small;
    
} __tt_aligned__(TT_CPU_BITBYTE) tt_static_fixed_pool_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

/*! get the item head from pred, and if this pred if full, update it 
 *
 * @param pool          brief
 *
 * @return              tt_void
 */
static tt_pool_data_empty_head_ref_t tt_static_fixed_pool_malloc_pred(tt_static_fixed_pool_t* pool)
{
    // check
    tt_assert_and_check_return_val(pool, tt_null);

    // done
    tt_pool_data_empty_head_ref_t data_head = tt_null;
    do {
        // exists the pred index
        tt_check_break(pool->pred_index);

        // the pred
        tt_size_t pred_index = pool->pred_index - 1;
        tt_assert((pred_index << TT_CPU_SHIFT) < pool->item_maxn);

        // the predict data
        tt_size_t* data = (tt_size_t*)pool->used_info + pred_index;
        // full?
        tt_check_break((*data) + 1);

        // the free bit index
        tt_size_t index = (pred_index << TT_CPU_SHIFT) + tt_static_fixed_pool_find_first(*data);

        // out of rang
        if(index > pool->item_maxn)
        {
            // clear pred index
            pool->pred_index = 0;
            break;
        }
        // check
        tt_assert(!(tt_static_fixed_pool_used_bset(pool->used_info, index)));

        // the data
        data_head = (tt_pool_data_empty_head_ref_t)(pool->data + index * pool->item_space);

        // allocate it
        tt_static_fixed_pool_used_set1(pool->used_info, index);
        
        // pred full? and update it
        if(!(*data + 1))
        {
            // clear pred
            pool->pred_index = 0;
            // predict the next
            if(index + 1 < pool->item_maxn && !tt_static_fixed_pool_used_bset(pool->used_info, index + 1))
                tt_static_fixed_pool_cache_pred(pool, index + 1);
        }

    }while(0);

    return data_head;
    
}

/*! find the free index from used_info
 *
 * @param pool          brief
 *
 * @return              tt_void
 */
static tt_pool_data_empty_head_ref_t tt_static_fixed_pool_malloc_find(tt_static_fixed_pool_t* pool)
{
    // check
    tt_assert_and_check_return_val(pool, tt_null);

    // init
    tt_size_t* p = (tt_size_t*)pool->used_info;
    tt_size_t* e = (tt_size_t*)(pool->used_info + pool->info_size);

    // check used_info algin
    tt_assert_and_check_return_val( !(((tt_size_t)p) & (TT_CPU_BITBYTE - 1)), tt_null );

    // move sizeof(tt_size_t) bytes
    while(p < e && !(*p + 1)) p++;  //TODO need opt
    tt_check_return_val(p < e, tt_null);

    // find the free bit index
    tt_size_t index = (((tt_byte_t*)p - pool->used_info) << 3) + tt_static_fixed_pool_find_first(*p);
    tt_check_return_val(index < pool->item_maxn, tt_null);
    
    // allocate it
    tt_pool_data_empty_head_ref_t data =  (tt_pool_data_empty_head_ref_t)(pool->data + index * pool->item_space);
    tt_static_fixed_pool_used_set1(pool->used_info, index);

    // predict this index if not full?
    if((*p) + 1) tt_static_fixed_pool_cache_pred(pool, index);

    return data;
}

/*! print base info of the pool, used for debug
 *
 * @param pool          brief
 *
 * @return              tt_void
 */
static tt_void_t tt_static_fixed_pool_print_info(tt_static_fixed_pool_t* pool __tt_debug_decl__)
{
   // check
   tt_assert_and_check_return(pool); 

   // print it
   tt_trace_d("pool,%p, usedinfo,%p, data,%p, tail,%p, manx,%d, count,%d, pred,%d, itemsize,%d, itemspace,%d, at %s(), %lu, %s",
              pool, 
              pool->used_info, 
              pool->data, 
              pool->tail, 
              pool->item_maxn, 
              pool->item_count, 
              pool->pred_index,
              pool->item_size,
              pool->item_space
              __tt_debug_args__
   );
}



/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_static_fixed_pool_ref_t tt_static_fixed_pool_init(tt_byte_t* data, tt_size_t size, tt_size_t item_size, tt_bool_t for_small)
{
    // check
    tt_assert_and_check_return_val(data && size && item_size, tt_null);

    // algin 
    tt_size_t diff = tt_align((tt_size_t)data, TT_POOL_DATA_ALIGN) - (tt_size_t)data;
    tt_assert_and_check_return_val(size > diff + sizeof(tt_static_fixed_pool_t), tt_null);
    size -= diff;
    data += diff;

    // init pool
    tt_static_fixed_pool_t* pool = (tt_static_fixed_pool_t*)data;
    tt_memset(pool, 0, sizeof(tt_static_fixed_pool_t)); 

    // for small allocator?
    pool->for_small = !!for_small;
    pool->data_head_size = (!!for_small)? sizeof(tt_pool_data_head_t): sizeof(tt_pool_data_empty_head_t);
//#ifndef __tt_debug__
    // size of empty struct type is 1 !!!
    if(!for_small) pool->data_head_size = 0;
//#endif
    tt_assert_and_check_return_val(!(pool->data_head_size & (TT_POOL_DATA_ALIGN - 1)), tt_null);

//#ifdef __tt_debug__
    // patch used for checking underflow
//    tt_size_t patch = 1;
//#else
    tt_size_t patch = 0;
//#endif

    // init the item space
    pool->item_space = tt_align(pool->data_head_size + item_size + patch, TT_POOL_DATA_ALIGN);

    // init the used_info
    pool->used_info = (tt_byte_t*)tt_align((tt_size_t)&pool[1], TT_POOL_DATA_ALIGN);
    tt_assert_and_check_return_val(data + size > pool->used_info, tt_null);

    /* init the item maxn
     *
     * used_info + maxn * item_space < left
     * align8(maxn) / 8 + maxn * item_space < left
     * (maxn + 7) / 8 + maxn * item_space < left
     * (maxn / 8) + (7 / 8) + maxn * item_space < left
     * maxn * (1 / 8 + item_space) < left - (7 / 8)
     * maxn < (left - (7 / 8)) / (1 / 8 + item_space)
     * maxn < (left * 8 - 7) / (1 + item_space * 8)
     */
    pool->item_maxn = (((data + size - pool->used_info) << 3) - 7) / (1 + (pool->item_space << 3));
    tt_assert_and_check_return_val(pool->item_maxn, tt_null);

    // init the used info size
    pool->info_size = tt_align(pool->item_maxn, TT_CPU_BITSIZE) >> 3;
    tt_assert_and_check_return_val(pool->info_size && !(pool->info_size & (TT_POOL_DATA_ALIGN - 1)), tt_null);
    // clear used info 0
    tt_memset(pool->used_info, 0, pool->info_size);    

    // init data
    pool->data = (tt_byte_t*)tt_align((tt_size_t)pool->used_info + pool->info_size, TT_POOL_DATA_ALIGN);
    tt_assert_and_check_return_val(data + size > pool->data, tt_null);
    tt_assert_and_check_return_val(pool->item_maxn * pool->item_space <= (tt_size_t)(data + size - pool->data), tt_null); //TODO 

    // init tail
    pool->tail = pool->data + pool->item_space * pool->item_maxn;

    // init item size
    pool->item_size = item_size;

    // init the item count
    pool->item_count = 0;

    // init the predict index(32 item)
    pool->pred_index = 1;

    // print it
#if TT_TRACE_DEBUG
    tt_static_fixed_pool_print_info(pool __tt_debug_val__);
#endif

    return (tt_static_fixed_pool_ref_t)pool;
}

tt_void_t tt_static_fixed_pool_exit(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return(pool);

    // clear it
    tt_static_fixed_pool_clear(self);

    // eixt it
    tt_memset(pool, 0, sizeof(tt_static_fixed_pool_t));
}

tt_size_t tt_static_fixed_pool_size(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, 0);

    return pool->item_count;
}

tt_size_t tt_static_fixed_pool_maxn(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, 0);

    return pool->item_maxn;
}

tt_bool_t tt_static_fixed_pool_full(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_false);

    return pool->item_maxn == pool->item_count? tt_true: tt_false;
}

tt_bool_t tt_static_fixed_pool_null(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_false);

    return !pool->item_count? tt_true: tt_false;   
}

tt_void_t tt_static_fixed_pool_clear(tt_static_fixed_pool_ref_t self)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return(pool);

    // clear used info
    if(pool->used_info) tt_memset(pool->used_info, 0, pool->info_size);

    // clear size
    pool->item_count = 0;

    // init pred
    pool->pred_index = 1;

}

tt_pointer_t tt_static_fixed_pool_malloc(tt_static_fixed_pool_ref_t self __tt_debug_decl__)
{
    tt_static_fixed_pool_t* pool =  (tt_static_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_null);

    // done
    tt_pointer_t                  data = tt_null;
    tt_pool_data_empty_head_ref_t data_head = tt_null;

    do{
        // full
        tt_check_break(pool->item_count < pool->item_maxn);

        // debug, make pred not work
        // pool->pred_index = 0;

        // predict it?
        data_head =  tt_static_fixed_pool_malloc_pred(pool);

        // find it
        if(!data_head) data_head = tt_static_fixed_pool_malloc_find(pool);
        tt_check_break(data_head);

        // the real data
        data = (tt_byte_t*)data_head + pool->data_head_size;

        // save the real size
        if(pool->for_small) ((tt_pool_data_head_ref_t)data_head)->size = pool->item_size;

        // size++
        pool->item_count++;        

    }while(0);

    // check
    tt_assertf(data, "malloc(%lu) failed!", pool->item_size);
    tt_assertf(!(((tt_size_t)data) & (TT_POOL_DATA_ALIGN - 1)), "malloc(%lu): unaligned data: %p", pool->item_size, data);

    // print it
#if TT_TRACE_DEBUG
    tt_static_fixed_pool_print_info(pool __tt_debug_val__);
#endif

    return data;
}

tt_bool_t tt_static_fixed_pool_free(tt_static_fixed_pool_ref_t self, tt_pointer_t data __tt_debug_decl__)
{
    tt_static_fixed_pool_t*       pool = (tt_static_fixed_pool_t*)self;
    tt_pool_data_empty_head_ref_t data_head = (tt_pool_data_empty_head_ref_t)((tt_byte_t*)data - pool->data_head_size);

    // check
    tt_assert_and_check_return_val(pool && data_head && pool->item_space, tt_false);

    // done
    tt_bool_t ret = tt_false;
    do{
        // index
        tt_size_t index = ((tt_byte_t*)data - pool->data) / pool->item_space;

        // check
        tt_assertf_and_check_break((tt_byte_t*)data >= pool->data && (tt_byte_t*)data + pool->item_space <= pool->tail, 
        "data: %p not belong to pool: %p", data, pool);
        tt_assertf_and_check_break(!(((tt_byte_t*)data - pool->data) % pool->item_space), "data: %p is invalid", data);
        tt_assertf_and_check_break(pool->item_size, "double free data: %p", data);
        tt_assertf_and_check_break(tt_static_fixed_pool_used_bset(pool->used_info, index), "double free data: %p", data);
        
        // free it
        tt_static_fixed_pool_used_set0(pool->used_info, index);
        pool->item_count--;

        // pred it if no pred index
        if(!pool->pred_index) tt_static_fixed_pool_cache_pred(pool, index);
        
        // set true
        ret = tt_true;
    }while(0);

    // print it
#if TT_TRACE_DEBUG
    tt_static_fixed_pool_print_info(pool __tt_debug_val__);
#endif

    return ret;
}


