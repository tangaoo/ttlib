/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipaddr.h
 * @ingroup    network
 * @author     tango
 * @date       2021-01-25 
 * @brief      ipaddr.h file
 */

#ifndef TT_NETWORK_IPADDR_H
#define TT_NETWORK_IPADDR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "ipv4.h"
#include "ipv6.h"
#include "unixaddr.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// the ip address string data maxn
#define TT_IPADDR_CSTR_MAXN     (TT_IPV6_CSTR_MAXN + 20)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the ip address family enum
typedef enum __tt_ipaddr_family_e
{
    TT_IPADDR_FAMILY_NONE   = 0,
    TT_IPADDR_FAMILY_IPV4   = 1,
    TT_IPADDR_FAMILY_IPV6   = 2,
    TT_IPADDR_FAMILY_UNIX   = 3,

}tt_ipaddr_family_e;

// the ip address type
typedef struct __tt_ipaddr_t
{
    // family
    tt_uint8_t              family : 7;

    // have ip?
    tt_uint8_t              have_ip : 1;

    // port
    tt_uint16_t             port;

    // the address
    union 
    {
        // the ipv4
        tt_ipv4_t           ipv4;

        // the ipv6
        tt_ipv6_t           ipv6;

        // the unixaddr
        tt_unixaddr_t       unixaddr;
    }                       u;

}tt_ipaddr_t, *tt_ipaddr_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! clear the ip addr
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_clear(tt_ipaddr_ref_t ipaddr);

/*! copy the ip addr, fast the *ipaddr = *copy
 *
 * @param ipaddr      the ip addr
 * @param copy        the ip addr copy
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_copy(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t copy);

/*! is empty?
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_is_empty(tt_ipaddr_ref_t ipaddr);

/*! is equal
 *
 * @param ipaddr      the ip addr
 * @param other       the ip addr copy
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other);

/*! get the ip addr str
 *
 * @param ipaddr      the ip addr
 * @param data        the ip addr str data
 * @param maxn        the ip addr str max length
 *
 * @return            the ipaddr const addr
 */
tt_char_t const*      tt_ipaddr_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn);

/*! set the ip addr form string
 *
 * @param ipaddr      the ip addr
 * @param cstr        the ip addr str data
 * @param port        the ip addr str max length
 * @param family      the ip addr str max length
 *
 * @return            the ipaddr const addr
 */
tt_bool_t             tt_ipaddr_cstr_set(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_int16_t port, tt_int8_t family);

/*! clear ip 
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_ip_clear(tt_ipaddr_ref_t ipaddr);


/*! the ip is empty?
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_ip_is_empty(tt_ipaddr_ref_t ipaddr);

/*! the ip is any?
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_ip_is_any(tt_ipaddr_ref_t ipaddr);

/*! the ip is loopback?
 *
 * @param ipaddr      the ip addr
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_ip_is_loopback(tt_ipaddr_ref_t ipaddr);

/*! the ip is equal?
 *
 * @param ipaddr      the ip addr
 * @param other       the ip addr
 *
 * @return            tt_true_t or tt_false_t
 */
tt_bool_t             tt_ipaddr_ip_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other);

/*! stringify ip 
 *
 * @param ipaddr      the addr, only analyze format if be null
 * @param data        the ip addr str data
 * @param maxn        max number string
 *
 * @return            tt_true_t or tt_false_t 
 */
tt_char_t const*      tt_ipaddr_ip_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn);

/*! only set ip address
 *
 * @param ipaddr      the ip addr
 * @param other       the other addr with ip data, clear it if be none
 *
 * @return            tt_void_t
 */
tt_bool_t             tt_ipaddr_ip_cstr_set(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_uint8_t family);

/*! get the ipv4
 *
 * @param ipaddr      the ip addr
 *
 * @return            ipv4
 */
tt_ipv4_ref_t         tt_ipaddr_ipv4(tt_ipaddr_ref_t ipaddr);

/*! set the ipv4
 *
 * @param ipaddr      the ip addr
 * @param ipv4        the ipv4, clear it if be null
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_ipv4_set(tt_ipaddr_ref_t ipaddr, tt_ipv4_ref_t ipv4);

/*! get the ipv6
 *
 * @param ipaddr      the ip addr
 *
 * @return            ipv6
 */
tt_ipv6_ref_t         tt_ipaddr_ipv6(tt_ipaddr_ref_t ipaddr);

/*! set the ipv6
 *
 * @param ipaddr      the ip addr
 * @param ipv6        the ipv6, clear it if be null
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_ipv6_set(tt_ipaddr_ref_t ipaddr, tt_ipv6_ref_t ipv6);

/*! get the unix socket path
 *
 * @param ipaddr      the ip addr
 *
 * @return            unixaddr
 */
tt_unixaddr_ref_t     tt_ipaddr_unix(tt_ipaddr_ref_t ipaddr);

/*! set the unix socket path
 *
 * @param ipaddr      the ip addr
 * @param unixaddr    the unixaddr
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_unix_set(tt_ipaddr_ref_t ipaddr, tt_unixaddr_ref_t unixaddr);

/*! set the unix socket path from string
 *
 * @param ipaddr      the ip addr
 * @param cstr        the unix addr string 
 * @param is_abstract true if the address is abstract address
 *
 * @return            tt_false or tt_true
 */
tt_bool_t             tt_ipaddr_unix_set_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_bool_t is_abstract);

/*! get the family
 *
 * @param ipaddr      the ip addr
 *
 * @return            the family
 */
tt_size_t             tt_ipaddr_family(tt_ipaddr_ref_t ipaddr);

/*! set the family
 *
 * @param ipaddr      the ip addr
 * @param family      the family
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_family_set(tt_ipaddr_ref_t ipaddr, tt_size_t family);

/*! get the port
 *
 * @param ipaddr      the ip addr
 *
 * @return            the port
 */
tt_size_t             tt_ipaddr_port(tt_ipaddr_ref_t ipaddr);

/*! set the port
 *
 * @param ipaddr      the ip addr
 * @param port        the port
 *
 * @return            tt_void_t
 */
tt_void_t             tt_ipaddr_port_set(tt_ipaddr_ref_t ipaddr, tt_size_t port);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif