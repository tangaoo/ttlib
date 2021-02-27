/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipv6.h
 * @ingroup    network
 * @author     tango
 * @date       2021-02-03 
 * @brief      ipv6.h file
 */

#ifndef TT_NETWORK_IPV6_H
#define TT_NETWORK_IPV6_H

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

// the ipv6 str maxn
#define TT_IPV6_CSTR_MAXN           (40 + 20)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the ipv6 type
 *
 * <pre>
 * xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx:xxxx
 * xxxx::xxxx:xxxx
 * ::ffff:xxx.xxx.xxx.xxx
 * ::fe80:1%1
 * </pre>
 */
typedef struct __tt_ipv6_t
{
    // the scope id
    tt_uint32_t         scope_id;

    // the addr
    union 
    {
        // u32 little-endian
        tt_uint32_t     u32[4];

        // u16 little-endian
        tt_uint16_t     u16[8];

        // u8
        tt_uint8_t      u8[16];

    }                   addr;
}tt_ipv6_t, *tt_ipv6_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

tt_void_t               tt_ipv6_clear(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_any(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_loopback(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_linklocal(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_mc_linklocal(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_sitelocal(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_multicast(tt_ipv6_ref_t ipv6);

tt_bool_t               tt_ipv6_is_equal(tt_ipv6_ref_t ipv6, tt_ipv6_ref_t other);

tt_char_t const*        tt_ipv6_cstr(tt_ipv6_ref_t ipv6, tt_char_t* data, tt_size_t maxn);

tt_bool_t               tt_ipv6_cstr_set(tt_ipv6_ref_t ipv6, tt_char_t const* cstr);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif
