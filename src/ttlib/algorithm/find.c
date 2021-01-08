/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       find.c
 * @ingroup    algorithm
 * @author     tango
 * @date       2020-12-13 
 * @brief      find.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "find.h"
#include "find_if.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_size_t tt_find(tt_iterator_ref_t iterator, tt_size_t head, tt_size_t tail, tt_cpointer_t data)
{
    return tt_find_if(iterator, head, tail, tt_predicate_eq, data);
}

tt_size_t tt_find_all(tt_iterator_ref_t iterator, tt_cpointer_t data)
{
    return tt_find_all_if(iterator, tt_predicate_eq, data);
}