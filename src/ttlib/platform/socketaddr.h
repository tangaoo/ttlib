/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       sockaddr.h
 * @ingroup    platform
 * @author     tango
 * @date       2021-01-25 
 * @brief      sockaddr.h file
 */

#ifndef TT_PLATFORM_SOCKETADDR_H
#define TT_PLATFORM_SOCKETADDR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../network/network.h"
#include "native_memcpy.h"
#include "../utils/bits.h"
#include <netinet/in.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <string.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef struct sockaddr_un      tt_sockaddr_un_t, *tt_sockaddr_un_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! save the socket address to the ip address
 *
 * @param ipaddr               the ip address
 * @param saddr                the socket address
 *
 * @return                     the size of socket address
 */
static __tt_inline__ tt_size_t tt_sockaddr_save(tt_ipaddr_ref_t ipaddr, struct sockaddr_storage const* saddr)
{
    // check
    tt_assert_and_check_return_val(ipaddr && saddr, 0);

    // clear it
    tt_ipaddr_clear(ipaddr); 

    // done 
    tt_size_t size = 0;
    switch (saddr->ss_family)
    {
    case AF_INET:
        {
            // the ipv4 addr
            struct sockaddr_in* addr4 = (struct sockaddr *)saddr;

            // save family
            tt_ipaddr_family_set(ipaddr, TT_IPADDR_FAMILY_IPV4);

            // make ipv4
            tt_ipv4_t ipv4;
            ipv4.u32 = (tt_uint32_t)addr4->sin_addr.s_addr;
            tt_ipaddr_ipv4_set(ipaddr, &ipv4);

            // save port 
            tt_ipaddr_port_set(ipaddr, tt_bits_be_to_ne_u16(addr4->sin_port));

            // save size
            size = sizeof(struct sockaddr);
        }
        break;
    case AF_INET6:
        {
            tt_trace_noimpl();
        }
        break;

    case AF_UNIX:
        {
            tt_sockaddr_un_ref_t addru = (tt_sockaddr_un_ref_t)saddr;

            // save family
            tt_ipaddr_family_set(ipaddr, TT_IPADDR_FAMILY_UNIX);

            // make unixaddr
            tt_unixaddr_t unixaddr;
            if(addru->sun_path[0])
            {
                strcpy(unixaddr.path, addru->sun_path);
                unixaddr.is_abstract = tt_false;
            }
            else
            {
                strcpy(unixaddr.path, &addru->sun_path[1]);
                unixaddr.is_abstract = tt_true;
            }

            // save unixaddr
            tt_ipaddr_unix_set(ipaddr, &unixaddr);

            // size
            size = sizeof(tt_sockaddr_un_t);
        }
        break;

    default:
        tt_trace_a("save ip");
        break;
    }

    return size;
}

/*! load the ip address to socket address
 *
 * @param ipaddr               the ip address
 * @param saddr                the socket address
 *
 * @return                     the size of socket address
 */
static __tt_inline__ tt_size_t tt_sockaddr_load(struct sockaddr_storage* saddr, tt_ipaddr_ref_t ipaddr)
{
    // check
    tt_assert_and_check_return_val(saddr && ipaddr, 0);

    // clear socket addr
    tt_memset(saddr, 0, sizeof(struct sockaddr_storage));

    // done
    tt_size_t size = 0;
    switch(ipaddr->family)
    {
    case TT_IPADDR_FAMILY_IPV4:
        {
            // the ipv4
            struct sockaddr_in* ipv4 = (struct sockaddr_in *)saddr;

            // load family
            ipv4->sin_family = AF_INET;

            // load address
            ipv4->sin_addr.s_addr = tt_ipaddr_ip_is_any(ipaddr) ? INADDR_ANY : ipaddr->u.ipv4.u32;

            // load port
            ipv4->sin_port = tt_bits_ne_to_be_u16(ipaddr->port);

            // size
            size = sizeof(struct sockaddr_in);
        }
        break;
    case TT_IPADDR_FAMILY_IPV6:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_IPADDR_FAMILY_UNIX:
        {
            // make unixaddr
            tt_sockaddr_un_ref_t addru = (tt_sockaddr_un_ref_t)saddr;

            // save addru
            if(!ipaddr->u.unixaddr.is_abstract)
            {
                strcpy(addru->sun_path, &ipaddr->u.unixaddr.path);
            }
            else
            {
                strcpy(&addru->sun_path[1], &ipaddr->u.unixaddr.path);
                addru->sun_path[0] = '\0';
            }

            // size
            size = sizeof(tt_sockaddr_un_t);
        }
        break;
    default:
        tt_trace_a("load socket address");
        break;
    }

    return size;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif