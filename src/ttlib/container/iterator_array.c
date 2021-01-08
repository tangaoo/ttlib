/*
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       iterator_array.c
 * @ingroup    container
 * @author     tango
 * @date       2020-11-24 
 * @brief      iterator_array.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "iterator_array.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * pravite implementation for pointer
 */

static tt_size_t tt_array_iterator_ptr_size(tt_iterator_ref_t iterator)
{
    // assert 
    tt_assert(iterator);
    
    // the size
    return ((tt_iterator_array_ref_t)iterator)->count;
}

static tt_size_t tt_array_iterator_ptr_head(tt_iterator_ref_t iterator)
{
    // assert 
    tt_assert(iterator);
    
    // the head 
    return 0;
}

static tt_size_t tt_array_iterator_ptr_tail(tt_iterator_ref_t iterator)
{
    // assert 
    tt_assert(iterator);
    
    // the tail 
    return ((tt_iterator_array_ref_t)iterator)->count;
}

static tt_size_t tt_array_iterator_ptr_next(tt_iterator_ref_t iterator, tt_size_t itor)
{
    // assert 
    tt_assert(iterator && itor < tt_array_iterator_ptr_size(iterator));
    
    // the next 
    return itor + 1;
}

static tt_size_t tt_array_iterator_ptr_prev(tt_iterator_ref_t iterator, tt_size_t itor)
{
    // assert 
    tt_assert(iterator && itor < tt_array_iterator_ptr_size(iterator));
    
    // the prev 
    return itor - 1;
}

static tt_pointer_t tt_array_iterator_ptr_item(tt_iterator_ref_t iterator, tt_size_t itor)
{
    // assert 
    tt_assert(iterator && itor < tt_array_iterator_ptr_size(iterator));
    
    // the item 
    return ((tt_pointer_t*)((tt_iterator_array_ref_t)iterator)->items)[itor];
}

static tt_int32_t tt_array_iterator_ptr_comp(tt_iterator_ref_t iterator, tt_cpointer_t litem, tt_cpointer_t ritem)
{
    // the compare 
    return (litem < ritem)? -1 : (litem > ritem); 
}

static tt_void_t tt_array_iterator_ptr_copy(tt_iterator_ref_t iterator, tt_size_t itor, tt_cpointer_t item)
{
    // assert 
    tt_assert(iterator && itor < tt_array_iterator_ptr_size(iterator));
    
    // the copy 
    ((tt_pointer_t*)((tt_iterator_array_ref_t)iterator)->items)[itor] = (tt_pointer_t)item;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_iterator_ref_t       tt_iterator_array_init_ptr(tt_iterator_array_ref_t iterator, tt_pointer_t* items, tt_size_t count)
{
    // assert
    tt_assert(iterator && items && count);
    
    // init op
    static tt_iterator_op_t op = 
    {
        tt_array_iterator_ptr_size,
        tt_array_iterator_ptr_head,
        tt_null,
        tt_array_iterator_ptr_tail,
        tt_array_iterator_ptr_prev,
        tt_array_iterator_ptr_next,
        tt_array_iterator_ptr_item,
        tt_array_iterator_ptr_comp,
        tt_array_iterator_ptr_copy,
        tt_null,
        tt_null
    };

    // init iterator
    iterator->base.priv = tt_null;
    iterator->base.mode = TT_ITERATOR_MODE_FORWARD | TT_ITERATOR_MODE_REVERSE | TT_ITERATOR_MODE_RACCESS | TT_ITERATOR_MODE_MUTABLE;
    iterator->base.step = sizeof(tt_pointer_t);
    iterator->base.op   = &op;
    iterator->items     = items;
    iterator->count     = count;

    // ok?
    return (tt_iterator_ref_t)iterator;
}


