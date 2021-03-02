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
 * @param type          the socket type, @ref tt_socket_type_e
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_pair(tt_size_t type, tt_socket_ref_t pair[2]);

/*! ctrl the socket
 *
 * @param socket        the socket
 * @param ctrl          the ctrl code
 *
 * @return              tt_true or tt_fasle
 */
tt_bool_t               tt_socket_ctrl(tt_socket_ref_t socket, tt_size_t ctrl, ...);

/*! connet the given server address
 *
 * @param socket        the socket
 * @param addr          the server address
 *
 * @return              ok: 1, continue: 0, failed: -1
 */
tt_long_t               tt_socket_connect(tt_socket_ref_t socket, tt_ipaddr_ref_t addr);

/*! bind the socket
 *
 *  can call tt_socket_local for bound address 
 * 
 * @param socket        the socket
 * @param addr          the address
 *                      - bind any port if port == 0
 *                      - bind any ip address if ip is empty
 *
 * @return              tt_void
 */
tt_bool_t               tt_socket_bind(tt_socket_ref_t socket, tt_ipaddr_ref_t addr);

/*! listen the socket
 *
 * @param socket        the socket
 * @param backlog       the maximum length for queue of pending connections 
 *
 * @return              tt_true of tt_false
 */
tt_bool_t               tt_socket_listen(tt_socket_ref_t socket, tt_size_t backlog);

/*! accept socket
 *
 * @param socket        the socket
 * @param addr          the client address
 *
 * @return              the client socket
 */
tt_socket_ref_t         tt_socket_accept(tt_socket_ref_t socket, tt_ipaddr_ref_t addr);

/*! get the local address
 *
 * @param socket        the socket
 * @param addr          the local address
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_local(tt_socket_ref_t socket, tt_ipaddr_ref_t addr);

/*! kill the socket
 *
 * @param socket        the socket 
 * @param mode          the mode 
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_kill(tt_socket_ref_t socket, tt_size_t mode);

/*! recv the socket data for tcp
 *
 * @param socket        the socket
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_recv(tt_socket_ref_t socket, tt_byte_t* data, tt_size_t size);

/*! send the socket data for tcp
 *
 * @param socket        the socket
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_send(tt_socket_ref_t socket, tt_byte_t const* data, tt_size_t size);

/*! recv the socket data for tcp with block mode
 *
 * @param socket        the socket
 * @param data          the data
 * @param size          the size
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_brecv(tt_socket_ref_t socket, tt_byte_t* data, tt_size_t size);

/*! send the socket data for tcp with block mode
 *
 * @param socket        the socket
 * @param data          the data
 * @param size          the size
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_socket_bsend(tt_socket_ref_t socket, tt_byte_t const* data, tt_size_t size);

#if 0
/*! recvv the socket data for tcp
 *
 * @param socket        the socket
 * @param list          the iovec list
 * @param size          the iovec size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_recvv(tt_socket_ref_t socket, tt_iovec_t* list, tt_size_t size);

/*! sendv the socket data for tcp
 *
 * @param socket        the socket
 * @param list          the iovec list
 * @param size          the iovec size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_sendv(tt_socket_ref_t socket, tt_iovec_t const* list, tt_size_t size);
#endif

/*! send file data
 *
 * @param socket        the socket
 * @param file          the file
 * @param offset        the offset
 *
 * @return              the real size or -1
 */
tt_hong_t               tt_socket_sendf(tt_socket_ref_t socket, tt_file_ref_t file, tt_hize_t offset,  tt_hize_t size);

/*! urecv the socket data for udp
 *
 * @param socket        the socket
 * @param addr          the peer address(output)
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_urecv(tt_socket_ref_t socket, tt_ipaddr_ref_t addr, tt_byte_t* data, tt_size_t size);

/*! usend the socket data for udp
 *
 * @param socket        the socket
 * @param addr          the address
 * @param data          the data
 * @param size          the size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_usend(tt_socket_ref_t socket, tt_ipaddr_ref_t addr, tt_byte_t const* data, tt_size_t size);

#if 0
/*! urecvv the socket data for udp
 *
 * @param socket        the socket
 * @param addr          the peer address(output)
 * @param list          the iovec list
 * @param size          the iovec size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_urecvv(tt_socket_ref_t socket, tt_ipaddr_ref_t addr, tt_iovec_t* list, tt_size_t size);

/*! usendv the socket data for udp
 *
 * @param socket        the socket
 * @param addr          the address
 * @param list          the iovec list
 * @param size          the iovec size
 *
 * @return              the real size or -1
 */
tt_long_t               tt_socket_usendv(tt_socket_ref_t socket, tt_ipaddr_ref_t addr, tt_iovec_t const* data, tt_size_t size);
#endif

/*! wait socket events
 *
 * @param socket        the socket
 * @param events        the socket events
 * @param timeout       the timeout, infinity: -1
 *
 * @return              > 0: the events code, 0: timeout or interrupted, -1: failed
 */
tt_long_t               tt_socket_wait(tt_socket_ref_t socket, tt_size_t events, tt_long_t timeout);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif