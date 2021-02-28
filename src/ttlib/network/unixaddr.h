/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       unixaddr.h
 * @ingroup    network
 * @author     tango
 * @date       2021-01-31 
 * @brief      unixaddr.h file
 */

#ifndef TT_NETWORK_UNIXADDR_H
#define TT_NETWORK_UNIXADDR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the unixaddr str data maxn
#define TT_UNIXADDR_CSTR_MAXN       (108)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the unix type
 *  eg.  /tmp/to.sock
 */
typedef struct __tt_unixaddr_t
{
    // is abstract
    tt_bool_t       is_abstract;

    // the path
    tt_char_t       path[TT_UNIXADDR_CSTR_MAXN];
}tt_unixaddr_t, *tt_unixaddr_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! clear the unixaddr
 *
 * @param unixaddr      the unixaddr
 *
 * @return              tt_void_t
 */
tt_void_t               tt_unixaddr_clear(tt_unixaddr_ref_t unixaddr);

/*! is abstract 
 *
 * @param unixaddr      the unixaddr
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_unixaddr_is_abstract(tt_unixaddr_ref_t unixaddr);

/*! is equal
 *
 * @param unixaddr      the unixaddr
 * 
 * @param other         the other unixaddr
 *
 * @return              tt_void_t
 */
tt_bool_t               tt_unixaddr_is_equal(tt_unixaddr_ref_t unixaddr, tt_unixaddr_ref_t other);

/*! unixaddr to cstr
 *
 * @param unixaddr      the unixaddr
 * 
 * @param data          the unixaddr data
 * 
 * @param maxn          the data max size
 *
 * @return              the const str
 */
tt_char_t const*        tt_unixaddr_cstr(tt_unixaddr_ref_t unixaddr, tt_char_t* data, tt_size_t maxn);

/*! clear the unixaddr
 *
 * @param unixaddr      the unixaddr
 * 
 * @param cstr          the cstr
 * 
 * @param is_abstract   the is_abstract
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_unixaddr_cstr_set(tt_unixaddr_ref_t unixaddr, tt_char_t const* cstr, tt_bool_t is_abstract);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif