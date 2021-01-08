/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       binary_find_if.c
 * @ingroup    algorithm
 * @author     tango
 * @date       2020-11-27 
 * @brief      binary_find_if.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "binary_find_if.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_size_t tt_binary_find_if(tt_iterator_ref_t iterator, tt_size_t head, tt_size_t tail, tt_iterator_comp_t comp, tt_cpointer_t priv)
{
    // check
    tt_assert_and_check_return_val(comp && iterator && (iterator->mode & TT_ITERATOR_MODE_RACCESS), tt_iterator_tail(iterator));

    // null?
    tt_assert_and_check_return_val(head != tail, tt_iterator_tail(iterator));

    // find
    tt_size_t l = head;
    tt_size_t r = tail;
    tt_size_t m = (l + r) >> 1;
    tt_long_t c = -1;
    while(l < r)
    {
        c = comp(iterator, tt_iterator_item(iterator, m), priv);
        // items(m) > priv, move r to m
        if(c > 0) r = m;
        // items(m) < priv, move l to m+1
        else if(c < 0) l = m + 1;
        // ==
        else break;

        // updata m
        m = (l + r) >> 1;
    }

    // ok?
    return !c? m: tt_iterator_tail(iterator);
}

tt_size_t  tt_binary_find_all_if(tt_iterator_ref_t iterator, tt_iterator_comp_t comp, tt_cpointer_t priv)
{
    return tt_binary_find_if(iterator, tt_iterator_head(iterator), tt_iterator_tail(iterator), comp, priv);
}


