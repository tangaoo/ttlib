/*!the tt library
 *
 * @copyright (c) 2019-2021, ttlib
 *
 * @file       predicate.c
 * @ingroup    algorithm
 * @author     tango
 * @date       2020-12-11 
 * @brief      predicate.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "predicate.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_bool_t tt_predicate_eq(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // assert
    tt_assert(iterator);
    
    // item == data?
    return !tt_iterator_comp(iterator, item, data);
}

tt_bool_t tt_predicate_bq(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // assert
    tt_assert(iterator);
    
    // item > data?
    return tt_iterator_comp(iterator, item, data) > 0;
}

tt_bool_t tt_predicate_lq(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // assert
    tt_assert(iterator);
    
    // item < data?
    return tt_iterator_comp(iterator, item, data) < 0;
}

tt_bool_t tt_predicate_beq(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // assert
    tt_assert(iterator);
    
    // item >= data?
    return tt_iterator_comp(iterator, item, data) >= 0;
}

tt_bool_t tt_predicate_leq(tt_iterator_ref_t iterator, tt_cpointer_t item, tt_cpointer_t data)
{
    // assert
    tt_assert(iterator);
    
    // item <= data?
    return tt_iterator_comp(iterator, item, data) <= 0;
}


