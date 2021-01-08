/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       binary_find.c
 * @ingroup    algorithm
 * @author     tango
 * @date       2020-12-03 
 * @brief      binary_find.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "binary_find.h"
#include "binary_find_if.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_size_t tt_binary_find(tt_iterator_ref_t iterator, tt_size_t head, tt_size_t tail, tt_cpointer_t item)
{
    return tt_binary_find_if(iterator, head, tail, tt_iterator_comp, item);
}

tt_size_t tt_binary_find_all(tt_iterator_ref_t iterator, tt_cpointer_t item)
{
    return tt_binary_find_all_if(iterator, tt_iterator_comp, item);
}