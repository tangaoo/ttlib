/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipv4.h
 * @ingroup    network
 * @author     tango
 * @date       2021-01-26 
 * @brief      ipv4.h file
 */

#ifndef TT_NETWORK_IPV4_H
#define TT_NETWORK_IPV4_H

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

// the ipv4 string data maxn
#define TT_IPV4_CSTR_MAXN               (16)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the ipv4 type
 *
 * xxx.xxx.xxx.xxx
 */
typedef union __tt_ipv4_t
{
    // u32, little endian
    tt_uint32_t     u32;

    // u16
    tt_uint16_t     u16[2];

    // u8
    tt_uint8_t      u8[4];
}tt_ipv4_t, *tt_ipv4_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! clear ipv4 
 *
 * @param ipv4          the ipv4
 *
 * @return              tt_void_t
 */
tt_void_t               tt_ipv4_clear(tt_ipv4_ref_t ipv4);

/*! is any address?
 *
 * @param ipv4          the ipv4
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_ipv4_is_any(tt_ipv4_ref_t ipv4);

/*! is loopback address?
 *
 * @param ipv4          the ipv4
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_ipv4_is_loopback(tt_ipv4_ref_t ipv4);


/*! is equal?
 *
 * @param ipv4          the ipv4
 * @param other         the other ipv4
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_ipv4_is_equal(tt_ipv4_ref_t ipv4, tt_ipv4_ref_t other);

/*! the ipv4 to const string 
 *
 * @param ipv4          the ipv4
 * @param data          the ipv4 string
 * @param maxn          the string max size
 *
 * @return              the ipv4 address by string
 */
tt_char_t const*        tt_ipv4_cstr(tt_ipv4_ref_t ipv4, tt_char_t* data, tt_size_t maxn);

/*! the const string to ipv4 
 *
 * @param ipv4          the ipv4
 * @param cstr          the cstr
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_ipv4_cstr_set(tt_ipv4_ref_t ipv4, tt_char_t const* cstr);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif