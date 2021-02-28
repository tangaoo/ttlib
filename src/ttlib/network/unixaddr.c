/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       unixaddr.c
 * @ingroup    network
 * @author     tango
 * @date       2021-02-01 
 * @brief      unixaddr.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME        "unixaddr"
#define TT_TRACE_MODULE_DEBUG       (0) 

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "unixaddr.h"
#include "../platform/native_memcpy.h"
#include <string.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_void_t tt_unixaddr_clear(tt_unixaddr_ref_t unixaddr)
{
    // check
    tt_assert_and_check_return(unixaddr);

    // clear it
    tt_memset(unixaddr->path, 0, sizeof(unixaddr->path));
}

tt_bool_t  tt_unixaddr_is_abstract(tt_unixaddr_ref_t unixaddr)
{
    // check
    tt_assert_and_check_return_val(unixaddr, tt_false);

    // is abstract?
    return unixaddr->is_abstract;
}

tt_bool_t tt_unixaddr_is_equal(tt_unixaddr_ref_t unixaddr, tt_unixaddr_ref_t other)
{
    // check
    tt_assert_and_check_return_val(unixaddr && other, tt_false);

    // equal?
    return (strcmp(unixaddr->path, other->path) == 0);
}

tt_char_t const* tt_unixaddr_cstr(tt_unixaddr_ref_t unixaddr, tt_char_t* data, tt_size_t maxn)
{
    // check
    tt_assert_and_check_return_val(unixaddr && data && maxn >= TT_UNIXADDR_CSTR_MAXN, tt_null);

    // done
    tt_size_t size = snprintf(data, maxn - 1, "%s", unixaddr->path);
    if (size >=0) data[size] = '\0';

    // ok
    return data;
}

tt_bool_t tt_unixaddr_cstr_set(tt_unixaddr_ref_t unixaddr, tt_char_t const* cstr, tt_bool_t is_abstract)
{
    // check
    tt_assert_and_check_return_val(unixaddr && cstr, tt_false);

    // set abstract
    unixaddr->is_abstract = is_abstract;

    // string copy
    strncpy(unixaddr->path, cstr, TT_UNIXADDR_CSTR_MAXN);

    // ok?
    return tt_true;
}