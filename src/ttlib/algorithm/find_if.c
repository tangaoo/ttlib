/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       find_if.c
 * @ingroup    algorithm
 * @author     tango
 * @date       2020-12-11 
 * @brief      find_if.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "find_if.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_size_t tt_find_if(tt_iterator_ref_t iterator, tt_size_t head, tt_size_t tail, tt_predicate_func_t pred, tt_cpointer_t data)
{
    // check 
    tt_assert_and_check_return_val(pred && iterator && (iterator->mode & TT_ITERATOR_MODE_FORWARD), tt_iterator_tail(iterator));

    // null
    tt_assert_and_check_return_val(head != tail, tt_iterator_tail(iterator));
    
    // done
    tt_size_t itor = head;
    tt_bool_t find = tt_false;
    for(; itor < tt_iterator_tail(iterator); itor = tt_iterator_next(iterator, itor))
    {
        if((find = pred(iterator, tt_iterator_item(iterator, itor), data))) break;
    }

    // ok?
    return find? itor: tt_iterator_tail(iterator);
}

tt_size_t tt_find_all_if(tt_iterator_ref_t iterator, tt_predicate_func_t pred, tt_cpointer_t data)
{
    // check 
    tt_assert_and_check_return_val(pred && iterator && (iterator->mode & TT_ITERATOR_MODE_FORWARD), tt_iterator_tail(iterator));

    // find
    return tt_find_if(iterator, tt_iterator_head(iterator), tt_iterator_tail(iterator), pred, data);
}