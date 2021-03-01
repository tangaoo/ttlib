/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       socket.h
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-01 
 * @brief      socket.h file
 */

#ifndef TT_PLATFORM_SOCKET_H
#define TT_PLATFORM_SOCKET_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../network/ipaddr.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// socket type enum
typedef enum __tt_socket_type_e
{
    TT_SOCKET_TYPE_NONE             = 0,

    // socket type
    TT_SOCKET_TYPE_SOCKET_STREAM    = 1 << 8,
    TT_SOCKET_TYPE_SOCKET_DGRAM     = 2 << 8,
    TT_SOCKET_TYPE_SOCKET_RAW       = 3 << 8,

    // socket ip protocol
    TT_SOCKET_TYPE_IPPROTO_TCP      = 1,
    TT_SOCKET_TYPE_IPPROTO_UDP      = 2,
    TT_SOCKET_TYPE_IPPROTO_ICMP     = 3,

    // socket for tcp
    TT_SOCKET_TYPE_TCP              = TT_SOCKET_TYPE_SOCKET_STREAM | TT_SOCKET_TYPE_IPPROTO_TCP,
    TT_SOCKET_TYPE_UDP              = TT_SOCKET_TYPE_SOCKET_DGRAM  | TT_SOCKET_TYPE_IPPROTO_UDP,
    TT_SOCKET_TYPE_ICMP             = TT_SOCKET_TYPE_SOCKET_RAW    | TT_SOCKET_TYPE_IPPROTO_ICMP,

}tt_socket_type_e;

// socket kill enum
typedef enum __tt_socket_kill_e
{
    TT_SOCKET_KILL_RO               = 0,
    TT_SOCKET_KILL_WO               = 1,
    TT_SOCKET_KILL_RW               = 2

}tt_socket_kill_e;

// the socket ctrl enum
typedef enum __tt_socket_ctrl_e
{
    TT_SOCKET_CTRL_SET_BLOCK            = 0,
    TT_SOCKET_CTRL_GET_BLOCK            = 1,
    TT_SOCKET_CTRL_SET_RECV_BUFF_SIZE   = 2,
    TT_SOCKET_CTRL_GET_RECV_BUFF_SIZE   = 3,
    TT_SOCKET_CTRL_SET_SEND_BUFF_SIZE   = 4,
    TT_SOCKET_CTRL_GET_SEND_BUFF_SIZE   = 5,
    TT_SOCKET_CTRL_SET_TCP_NODELAY      = 6,
    TT_SOCKET_CTRL_GET_TCP_NODELAY      = 7,
    TT_SOCKET_CTRL_SET_TCP_KEEPALIVE    = 8,
    TT_SOCKET_CTRL_SET_KEEPALIVE        = 9,
    TT_SOCKET_CTRL_SET_NOSIGPIPE        = 10

}tt_socket_ctrl_e;

// socket event enum
typedef enum __tt_socke_event_e
{
    TT_SOCKET_EVENT_NONE                = 0x0000,
    TT_SOCKET_EVENT_RECV                = 0x0001,
    TT_SOCKET_EVENT_SEND                = 0x0002,
    TT_SOCKET_EVENT_CONN                = TT_SOCKET_EVENT_SEND,
    TT_SOCKET_EVENT_ACPT                = TT_SOCKET_EVENT_RECV,
    TT_SOCKET_EVENT_EALL                = TT_SOCKET_EVENT_RECV | TT_SOCKET_EVENT_SEND


}tt_socket_event_e;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init socket
 *
 * @param type          the socket type 
 * @param family        the address family, default: ipv4 
 *
 * @return              the socket
 */
tt_socket_ref_t         tt_socket_init(tt_size_t type, tt_size_t family);

/*! exit socket
 *
 * @param socket        the socket
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_exit(tt_socket_ref_t socket);

/*! init socket pair
 *
 * @param a             brief
 *
 * @return              tt_void_t
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif