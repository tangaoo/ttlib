/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipaddr.c
 * @ingroup    network
 * @author     tango
 * @date       2021-02-21 
 * @brief      ipaddr.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ipaddr.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static __tt_inline__ tt_bool_t tt_ipaddr_ipv6_to_ipv4(tt_ipv6_ref_t ipv6, tt_ipv4_ref_t ipv4)
{}

static __tt_inline__ tt_bool_t tt_ipaddr_ipv4_to_ipv6(tt_ipv4_ref_t ipv4, tt_ipv6_ref_t ipv6)
{}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_void_t tt_ipaddr_clear(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return(ipaddr);

    // done
    ipaddr->family  = TT_IPADDR_FAMILY_NONE;
    ipaddr->port    = 0;
    ipaddr->have_ip = 0;
}

tt_void_t tt_ipaddr_copy(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t copy) 
{
    // check
    tt_assert_and_check_return(ipaddr && copy);

    // done
    if(!copy->have_ip)
    {
        ipaddr->family = copy->family;
        ipaddr->port   = copy->port;
    }
    // attempt to copy ipv4
    else if(copy->family == TT_IPADDR_FAMILY_IPV4)
    {
        ipaddr->family  = TT_IPADDR_FAMILY_IPV4;
        ipaddr->port    = copy->port;
        ipaddr->have_ip = 1;
        ipaddr->u.ipv4  = copy->u.ipv4;
    }
    else
        *ipaddr = *copy;
}

tt_bool_t tt_ipaddr_is_empty(tt_ipaddr_ref_t ipaddr) 
{
    // check
    tt_assert_and_check_return_val(ipaddr, tt_false);
    
    // done
    return !ipaddr->have_ip;
}

 
tt_bool_t             tt_ipaddr_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 

 
tt_char_t const*      tt_ipaddr_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn) 

 
tt_bool_t             tt_ipaddr_set(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_int16_t port, tt_int8_t family) 

 
tt_void_t             tt_ipaddr_ip_clear(tt_ipaddr_ref_t ipaddr) 


 
tt_bool_t             tt_ipaddr_ip_is_empty(tt_ipaddr_ref_t ipaddr) 

 
tt_bool_t             tt_ipaddr_ip_is_any(tt_ipaddr_ref_t ipaddr) 

 
tt_bool_t             tt_ipaddr_ip_is_loopback(tt_ipaddr_ref_t ipaddr) 

 
tt_bool_t             tt_ipaddr_ip_is_equal(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 

 
tt_char_t const*      tt_ipaddr_ip_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t* data, tt_size_t maxn) 

 
tt_char_t const*      tt_ipaddr_ip_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_uint8_t family) 

 
tt_void_t             tt_ipaddr_ip_set(tt_ipaddr_ref_t ipaddr, tt_ipaddr_ref_t other) 

 
tt_ipv4_ref_t         tt_ipaddr_ipv4(tt_ipaddr_ref_t ipaddr) 

 
tt_void_t             tt_ipaddr_ipv4_set(tt_ipaddr_ref_t ipaddr, tt_ipv4_ref_t ipv4) 

 
tt_ipv6_ref_t         tt_ipaddr_ipv6(tt_ipaddr_ref_t ipaddr) 

 
tt_void_t             tt_ipaddr_ipv6_set(tt_ipaddr_ref_t ipaddr, tt_ipv4_ref_t ipv6) 

 
tt_unixaddr_ref_t     tt_ipaddr_unix(tt_ipaddr_ref_t ipaddr) 

 
tt_void_t             tt_ipaddr_unix_set(tt_ipaddr_ref_t ipaddr, tt_unixaddr_ref_t unixaddr) 

 
tt_void_t             tt_ipaddr_unix_set_cstr(tt_ipaddr_ref_t ipaddr, tt_char_t const* cstr, tt_bool_t is_abstract) 

 
tt_size_t             tt_ipaddr_family(tt_ipaddr_ref_t ipaddr) 

 
tt_void_t             tt_ipaddr_family_set(tt_ipaddr_ref_t ipaddr, tt_size_t family) 

 
tt_size_t             tt_ipaddr_port(tt_ipaddr_ref_t ipaddr) 

 
tt_void_t             tt_ipaddr_port_set(tt_ipaddr_ref_t ipaddr, tt_size_t port) 








