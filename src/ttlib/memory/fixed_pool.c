/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       fixed_pool.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-24 
 * @brief      fixed_pool.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME          "FIXED_POOL"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "fixed_pool.h"
#include "impl/static_fixed_pool.h"
#include "../container/container.h"
#include "../algorithm/algorithm.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the item belong to this slot?
#define tt_fixed_pool_slot_exits(slot, item)          ( ((tt_byte_t*)(item) > (tt_byte_t*)(slot)) && ((tt_byte_t*)(item) < (tt_byte_t*)(slot) + (slot)->size) )

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the fixed pool slot type
typedef struct __tt_fixed_pool_slot_t
{
    // the size, sizeof(slot) + data
    tt_size_t                       size;

    // the static fixed pool
    tt_static_fixed_pool_ref_t      pool;

    // the list entry
    tt_list_entry_t                 entry;

}tt_fixed_pool_slot_t;

// the fixed pool type
typedef struct __tt_fixed_pool_impl_t
{
    // the large allocator
    tt_allocator_ref_t              large_allocator;

    // the slot size
    tt_size_t                       slot_size;

    // the item size
    tt_size_t                       item_size;

    // the item count used
    tt_size_t                       item_count;

    // the init func
    tt_fixed_pool_item_init_func_t  func_init;

    // the exit func
    tt_fixed_pool_item_exit_func_t  func_exit;

    // the func priv data
    tt_cpointer_t                   func_priv;

    // the current slot
    tt_fixed_pool_slot_t*           current_slots;

    // the full slot list
    tt_list_entry_head_t            full_slots;

    // the partial slot list
    tt_list_entry_head_t            partial_slots;

    // the slot array
    tt_fixed_pool_slot_t**          slot_list;
        
    // the slot count used of the slot_list
    tt_size_t                       slot_count;

    // the slot space of the slot_list
    tt_size_t                       slot_space;

    // for small allocator
    tt_bool_t                       for_small;

}tt_fixed_pool_t;


/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tt_bool_t tt_fixed_pool_item_exit(tt_pointer_t data, tt_cpointer_t priv)  // TODO 
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)priv;
    tt_assert(pool && pool->func_exit);

    // done exit
    pool->func_exit(data, pool->func_priv);

    return tt_true;
}

static tt_void_t tt_fixed_pool_slot_exit(tt_fixed_pool_t* pool, tt_fixed_pool_slot_t* slot)
{
    // check
    tt_assert_and_check_return(pool && pool->large_allocator && slot);
    tt_assert_and_check_return(pool->slot_list && pool->slot_count);
    
    // trace
    tt_trace_d("slot[%lu]: exit: size: %lu", pool->item_size, slot->size);

    // make the iterator
    tt_iterator_array_t iterator_array;
    tt_iterator_ref_t   iterator = tt_iterator_array_init_ptr(&iterator_array, (tt_pointer_t*)pool->slot_list, pool->slot_count);
    tt_assert(iterator);

    // find the slot of the slot list
    tt_size_t itor = tt_binary_find_all(iterator, (tt_cpointer_t)slot); //TODO need 
    tt_assert_and_check_return(itor != tt_iterator_tail(iterator) && itor < pool->slot_count && pool->slot_list[itor]);

    // remove the slot, just del a item of array, and move array
    if(itor < pool->slot_count-1) tt_memmov(pool->slot_list + itor, pool->slot_list + itor + 1, (pool->slot_count - itor - 1) * sizeof(tt_fixed_pool_slot_t*));

    // update the slot count
    pool->slot_count--;

    // exit slot
    tt_allocator_large_free(pool->large_allocator, slot);
}

static tt_fixed_pool_slot_t* tt_fixed_pool_slot_init(tt_fixed_pool_t* pool)
{
    // check
    tt_assert_and_check_return_val(pool && pool->large_allocator && pool->slot_size && pool->item_size, tt_null);

    // done
    tt_bool_t             ok = tt_false;
    tt_fixed_pool_slot_t* slot = tt_null;
    do {
//#ifdef __tt_debug__
//        tt_size_t patch = 1;
//#else
        tt_size_t patch = 0;
//#endif
        // the item space
        tt_size_t item_space = sizeof(tt_pool_data_head_t) + pool->item_size + patch;

        // the need space
        tt_size_t need_space = sizeof(tt_fixed_pool_slot_t) + pool->slot_size * item_space;

        // make slot
        tt_size_t real_space;
        slot = (tt_fixed_pool_slot_t*)tt_allocator_large_malloc(pool->large_allocator, need_space, &real_space);
        tt_assert_and_check_break(slot);
        tt_assert_and_check_break(real_space > sizeof(tt_fixed_pool_slot_t) + item_space);
        
        // init slot
        slot->size = real_space;
        slot->pool = tt_static_fixed_pool_init((tt_byte_t*)&slot[1], real_space - sizeof(tt_fixed_pool_slot_t), pool->item_size, pool->for_small);
        tt_assert_and_check_break(slot->pool);

        // no slot list?
        if(!pool->slot_list)
        {
            // init the slot list
            tt_size_t size = 0;
            pool->slot_list = (tt_fixed_pool_slot_t**)tt_allocator_large_nalloc(pool->large_allocator, 64, sizeof(tt_fixed_pool_slot_t*), &size);
            tt_assert_and_check_break(pool->slot_list && size);

            // init the slot count
            pool->slot_count = 0;

            // init the slot space
            pool->slot_space = size / sizeof(tt_fixed_pool_slot_t*);
            tt_assert_and_check_break(pool->slot_space);
        }
        // no enough space
        else if(pool->slot_count == pool->slot_space)
        {
            // grow is slot list
            tt_size_t size = 0;
            pool->slot_list = (tt_fixed_pool_slot_t**)tt_allocator_large_ralloc(pool->large_allocator, pool->slot_list, pool->slot_space << 1, &size);
            tt_assert_and_check_break(pool->slot_list && size);

            // update the slot space
            pool->slot_space = size / sizeof(tt_fixed_pool_slot_t*);
            tt_assert_and_check_break(pool->slot_space);
        }

        // check
        tt_assert_and_check_break(pool->slot_count < pool->slot_space);

        // insert slot to the slot list in the increasing order (TODO opt)
        tt_size_t i = 0;
        tt_size_t n = pool->slot_count;
        for(i = 0; i < n; i++)
            if(slot < pool->slot_list[i] )
                break;
        if(i < n)
            tt_memmov(pool->slot_list + i + 1, pool->slot_list + i, (n - i) * sizeof(tt_fixed_pool_slot_t*));
        pool->slot_list[i] = slot;

        // update the slot count
        pool->slot_count++;

        // trace
        tt_trace_d("slot[%lu]: init: size: %lu => %lu, item: %lu => %lu", pool->item_size, need_space, real_space, pool->slot_size, tt_static_fixed_pool_maxn(slot->pool));

        // ok
        ok = tt_true;

    } while(0);

    if(!ok)
    {
        // exit it
        if(slot) tt_fixed_pool_slot_exit(pool, slot);
        slot = tt_null;
    }

    return slot;

}

static tt_long_t tt_fixed_pool_slot_comp(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // check
    tt_assert(item);

    tt_fixed_pool_slot_t* slot = (tt_fixed_pool_slot_t*)item;

    // comp (item > data => 1; item = data => 0; item < data => -1)
    return (tt_byte_t*)slot > (tt_byte_t*)data? 1 : ((tt_byte_t*)slot + slot->size) > (tt_byte_t*)data? 0 : -1;

}

// fast find slot according to data
static tt_fixed_pool_slot_t* tt_fixed_pool_slot_find(tt_fixed_pool_t* pool, tt_pointer_t data)
{
    // check
    tt_assert_and_check_return_val(pool && data, tt_null);

    // make iterator_array
    tt_iterator_array_t iterator_array;
    tt_iterator_ref_t   iterator = tt_iterator_array_init_ptr(&iterator_array, (tt_pointer_t*)pool->slot_list, pool->slot_count);
    tt_assert(iterator);

    // find it
    tt_size_t itor = tt_binary_find_all_if(iterator, tt_fixed_pool_slot_comp, data);
    tt_check_return_val(itor != tt_iterator_tail(iterator), tt_null);

    // the slot
    tt_fixed_pool_slot_t* slot = pool->slot_list[itor];
    tt_assert_and_check_return_val(slot, tt_null);
    
    return slot;
    
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_fixed_pool_ref_t tt_fixed_pool_init_(tt_allocator_ref_t large_allocator, tt_size_t slot_size, tt_size_t item_size, tt_bool_t for_small, tt_fixed_pool_item_init_func_t item_init, tt_fixed_pool_item_exit_func_t item_exit, tt_cpointer_t priv)
{
    // check
    tt_assert_and_check_return_val(item_size, tt_null);

    // done
    tt_bool_t        ok = tt_false;
    tt_fixed_pool_t* pool = tt_null;
    do {
        // no large allocator
        if(!large_allocator) large_allocator =  tt_allocator();
        tt_assert_and_check_break(large_allocator);

        //make pool
        pool = (tt_fixed_pool_t*)tt_allocator_large_malloc0(large_allocator, sizeof(tt_fixed_pool_t), tt_null);
        tt_assert_and_check_break(pool);

        // init pool
        pool->large_allocator = large_allocator;
        pool->slot_size       = slot_size? slot_size : 256;
        pool->item_size       = item_size;
        pool->func_init       = item_init;
        pool->func_exit       = item_exit;
        pool->func_priv       = priv;
        pool->for_small       = for_small;
        tt_assert_and_check_break(pool->slot_size);

        // init the partial slot list
        tt_list_entry_init(&pool->partial_slots, tt_fixed_pool_slot_t, entry, tt_null);

        // init the full slot list
        tt_list_entry_init(&pool->full_slots, tt_fixed_pool_slot_t, entry, tt_null);
        
        // ok
        ok = tt_true;

    } while(0);
    
    if(!ok)
    {
        // exit pool
        if(pool) tt_fixed_pool_exit((tt_fixed_pool_ref_t)pool);
        pool = tt_null;
    }

    // ok?
    return (tt_fixed_pool_ref_t)pool;

}

tt_fixed_pool_ref_t tt_fixed_pool_init(tt_allocator_ref_t large_allocator, tt_size_t slot_size, tt_size_t item_size, tt_fixed_pool_item_init_func_t item_init, tt_fixed_pool_item_exit_func_t item_exit, tt_cpointer_t priv)
{
    return tt_fixed_pool_init_(large_allocator, slot_size, item_size, tt_false, item_init, item_exit, priv);
}

tt_void_t tt_fixed_pool_exit(tt_fixed_pool_ref_t self)
{
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return(pool);

    // clear it
    tt_fixed_pool_clear((tt_fixed_pool_ref_t)pool);

    // exit the current slot
    if(pool->current_slots) tt_fixed_pool_slot_exit(pool, pool->current_slots);
    pool->current_slots = tt_null;

    // exit the slot list
    if(pool->slot_list) tt_allocator_large_free(pool->large_allocator, pool->slot_list);
    pool->slot_list  = tt_null;
    pool->slot_count = 0;
    pool->slot_space = 0;
    
    // exit the pool
    tt_allocator_large_free(pool->large_allocator, pool);

}

tt_size_t tt_fixed_pool_size(tt_fixed_pool_ref_t self)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, 0);

    // the item count
    return pool->item_count;
    
}

tt_size_t tt_fixed_pool_item_size(tt_fixed_pool_ref_t self)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, 0);

    // the item size 
    return pool->item_size;
    
}

tt_void_t tt_fixed_pool_clear(tt_fixed_pool_ref_t self)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return(pool);
    
    // exit items
//    if (pool->func_exit) tt_fixed_pool_walk(self, tt_fixed_pool_item_exit, (tt_pointer_t)pool);

    // exit partial slots
    tt_iterator_ref_t partial_iterator = tt_list_entry_iterator(&pool->partial_slots); 
    if(partial_iterator)
    {
        // walk it
        tt_size_t itor = tt_iterator_head(partial_iterator);
        while(itor != tt_iterator_tail(partial_iterator))
        {
            // the slot
            tt_fixed_pool_slot_t* slot = tt_iterator_item(partial_iterator, itor);
            tt_assert_and_check_break(slot);

            // check
            tt_assert(slot != pool->current_slots);

            // save next
            tt_size_t next = tt_iterator_next(partial_iterator, itor);

            // exit slot
            tt_fixed_pool_slot_exit(pool, slot);

            // next
            itor = next;
        }
    }

    // exit full slots
    tt_iterator_ref_t full_iterator = tt_list_entry_iterator(&pool->full_slots); 
    if(full_iterator)
    {
        // walk it
        tt_size_t itor = tt_iterator_head(full_iterator);
        while(itor != tt_iterator_tail(full_iterator))
        {
            // the slot
            tt_fixed_pool_slot_t* slot = tt_iterator_item(full_iterator, itor);
            tt_assert_and_check_break(slot);

            // check
            tt_assert(slot != pool->current_slots);

            // save next
            tt_size_t next = tt_iterator_next(full_iterator, itor);

            // exit slot
            tt_fixed_pool_slot_exit(pool, slot);

            // next
            itor = next;
        }
    }

    // clear current slot
    if(pool->current_slots && pool->current_slots->pool)
        tt_static_fixed_pool_clear(pool->current_slots->pool);

    // clear itme count
    pool->item_count = 0;
    
    // clear list
    tt_list_entry_clear(&pool->partial_slots);
    tt_list_entry_clear(&pool->full_slots);

}

tt_pointer_t tt_fixed_pool_malloc_(tt_fixed_pool_ref_t self __tt_debug_decl__)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_null);

    // done
    tt_bool_t      ok = tt_false;
    tt_pointer_t data = tt_null;
    do {
        // no current slot or current slot is full? update the current slot.
        if(!pool->current_slots || tt_static_fixed_pool_full(pool->current_slots->pool))
        {
            // move the current slot to full slot if exists
            if(pool->current_slots) tt_list_entry_insert_tail(&pool->full_slots, &pool->current_slots->entry);

            // clear the current slot
            pool->current_slots = tt_null;

            // attempt to get a slot from partial slot list
            if(!tt_list_entry_is_null(&pool->partial_slots))
            {
                // the head entry
                tt_list_entry_ref_t entry = tt_list_entry_head(&pool->partial_slots);
                tt_assert_and_check_break(entry);

                // the head slot
                pool->current_slots = (tt_fixed_pool_slot_t*)tt_list_entry(&pool->partial_slots, entry);
                tt_assert_and_check_break(pool->current_slots);

                // remove this slot from partial slots
                tt_list_entry_remove(&pool->partial_slots, entry);
            }
            // make a new slot
            else pool->current_slots = tt_fixed_pool_slot_init(pool);
        }

        // check
        tt_assert_and_check_break(pool->current_slots && pool->current_slots->pool);
        tt_assert_and_check_break(!tt_static_fixed_pool_full(pool->current_slots->pool));

        // make data from current slots
        data = tt_static_fixed_pool_malloc(pool->current_slots->pool __tt_debug_args__);
        tt_assert_and_check_break(data);

        // done init
//        if (pool->func_init && !pool->func_init(data, pool->func_priv)) break;
        
        // update the item count
        pool->item_count++;

        // ok
        ok = tt_true;

    } while(0);

    if(!ok)
    {
        // exit data
        if(data && pool->current_slots && pool->current_slots->pool)
            tt_static_fixed_pool_free(pool->current_slots->pool, data __tt_debug_args__);
        data = tt_null;
    }

    // check
    tt_assertf(data, "malloc(%lu) failed!", pool->item_size);

    // ok?
    return data;
}

tt_pointer_t tt_fixed_pool_malloc0_(tt_fixed_pool_ref_t self __tt_debug_decl__)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_null);

    // malloc it
    tt_pointer_t data = tt_fixed_pool_malloc_((tt_fixed_pool_ref_t)pool __tt_debug_args__);
    tt_assert_and_check_return_val(data, tt_null);

    // clear it
    tt_memset(data, 0, pool->item_size);

    // ok
    return data;
}

tt_bool_t tt_fixed_pool_free_(tt_fixed_pool_ref_t self, tt_pointer_t data __tt_debug_decl__)
{
    // check
    tt_fixed_pool_t* pool = (tt_fixed_pool_t*)self;
    tt_assert_and_check_return_val(pool, tt_false);

    // done
    tt_bool_t ok = tt_false;
    do {
        // check
        tt_assertf_and_check_break(pool->item_count, "double free the data, %p", data);

        // find the slot
        tt_fixed_pool_slot_t* slot = tt_fixed_pool_slot_find(pool, data);
        tt_assertf_and_check_break(slot, "the data(%p) is not belong to the pool(%p)", data, pool);
        tt_assert_and_check_break(slot->pool);

        // the slot is full?
        tt_bool_t full = tt_static_fixed_pool_full(slot->pool);

        // done exit
        if (pool->func_exit) pool->func_exit(data, pool->func_priv);

        // free data
        if(!tt_static_fixed_pool_free(slot->pool, data __tt_debug_args__)) break;

        // not current slot?
        if(slot != pool->current_slots)
        {
            // the slot is full before free
            if(full)
            {
                // remove the slot from full slot list
                tt_list_entry_remove(&pool->full_slots, &slot->entry);

                // insert the slot to partial list
                tt_list_entry_insert_tail(&pool->partial_slots, &slot->entry);
            }
            // the slot is null?
            else if(tt_static_fixed_pool_null(slot->pool))
            {
                // remove the slot form partial slot list
                tt_list_entry_remove(&pool->partial_slots, &slot->entry);

                // exit the slot
                tt_fixed_pool_slot_exit(pool, slot);
            }
        }

        // update the item count
        pool->item_count--;

        // ok?
        ok = tt_true;

    } while(0);

    // not ok?
    if(!ok)
    {}

    return ok;

}


