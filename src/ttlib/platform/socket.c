/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       socket.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-01-13 
 * @brief      socket.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */

#include "prefix.h"
#include "socket.h"
#include "socketaddr.h"
#include "stdarg.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <signal.h>
#include <sys/uio.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
__tt_extern_c_enter__
tt_long_t tt_socket_wait_impl(tt_socket_ref_t sock, tt_size_t events, tt_long_t timeout);
__tt_extern_c_leave__

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */

static tt_int_t tt_socket_type(tt_size_t type)
{
    // get socket type
    switch ((type >> 8) << 8)
    {
    case TT_SOCKET_TYPE_SOCKET_STREAM:
        return SOCK_STREAM;
    case TT_SOCKET_TYPE_SOCKET_DGRAM:
        return SOCK_DGRAM;
    case TT_SOCKET_TYPE_SOCKET_RAW:
        return SOCK_RAW;
    }

    // failed
    return -1;
}

static tt_int_t tt_socket_proto(tt_size_t type, tt_size_t family)
{
    if(family == TT_IPADDR_FAMILY_UNIX) return 0;

    // get socket protocal 
    switch(type & 0xff)
    {
    case TT_SOCKET_TYPE_IPPROTO_TCP:
        return IPPROTO_TCP;
    case TT_SOCKET_TYPE_IPPROTO_UDP:
        return IPPROTO_UDP;
    case TT_SOCKET_TYPE_IPPROTO_ICMP:
        switch (family)
        {
        case TT_IPADDR_FAMILY_IPV4:
            return IPPROTO_ICMP;
        case TT_IPADDR_FAMILY_IPV6:
            return IPPROTO_ICMPV6;
        default:
            tt_trace_a("socket proto");
        }
    default:
        tt_trace_a("socket proto");
    }

    // failed
    return -1;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_bool_t tt_socket_init_env()
{
    // ignore sigpipe
    signal(SIGPIPE, SIG_IGN);

    return tt_true;
}

tt_void_t tt_socket_exit_env()
{}

tt_socket_ref_t tt_socket_init(tt_size_t type, tt_size_t family)
{
    // check
    tt_assert_and_check_return_val(type, tt_null);

    // done
    tt_socket_ref_t sock = tt_null;
    do
    {
        // init socket type and protocol
        tt_int_t t = tt_socket_type(type);
        tt_int_t p = tt_socket_proto(type, family);
        tt_assert_and_check_break(t >=0 && p >= 0);

        // init socket family
        tt_int_t f = (family == TT_IPADDR_FAMILY_UNIX)? AF_UNIX : (family == TT_IPADDR_FAMILY_IPV6) ? AF_INET6 : AF_INET;

        // socket
        tt_int_t fd = socket(f, t, p);
        tt_assert_and_check_break(fd >= 0);

        // set non-block mode
        fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

        // save socket
        sock = tt_fd2socket(fd);

    } while (0);

    // trace
    tt_trace_d("init: type, %lu, family, %lu, sock, %p", type, family, sock);

    // ok?
    return sock;
}

tt_bool_t tt_socket_pair(tt_size_t type, tt_socket_ref_t pair[2])
{
    // check
    tt_assert_and_check_return_val(type && pair, tt_false);

    // init socket type
    tt_int_t t = tt_socket_type(type);
    tt_assert_and_check_return_val(t >= 0, tt_false);

    // make pair (protocol must be 0)
    tt_int_t fd[2] = {0};
    if(socketpair(AF_LOCAL, t, 0, fd) == -1) return tt_false;

    // non-block
    fcntl(fd[0], F_SETFL, fcntl(fd[0], F_GETFL) | O_NONBLOCK);
    fcntl(fd[1], F_SETFL, fcntl(fd[1], F_GETFL) | O_NONBLOCK);

    // save pair
    pair[0] = tt_fd2socket(fd[0]);
    pair[1] = tt_fd2socket(fd[1]);

    tt_trace_d("pair: %p %p", pair[0], pair[1]);

    // ok
    return tt_true;
}

tt_bool_t tt_socket_ctrl(tt_socket_ref_t socket, tt_size_t ctrl, ...)
{
    // check
    tt_assert_and_check_return_val(socket, tt_false);

    // init args
    va_list args;
    va_start(args, ctrl);

    // done
    tt_int_t    fd = tt_socket2fd(socket);
    tt_bool_t   ok = tt_false;
    switch (ctrl)
    {
    case TT_SOCKET_CTRL_SET_BLOCK:
        {
            // set block
            tt_bool_t is_block = (tt_bool_t)va_arg(args, tt_bool_t);

            // block it
            if(is_block) fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) & ~O_NONBLOCK);
            else fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

            ok = tt_true;
        }
        break;
    case TT_SOCKET_CTRL_GET_BLOCK:
        {
            // set block
            tt_bool_t* pis_block = (tt_bool_t *)va_arg(args, tt_bool_t*);

            // block it
            *pis_block = (fcntl(fd, F_GETFL) & O_NONBLOCK)? tt_false : tt_true;

            ok = tt_true;
        }
        break;
    case TT_SOCKET_CTRL_SET_TCP_NODELAY:
        {
            // enable the nagle's algorithm
            tt_int_t enable = (tt_int_t)va_arg(args, tt_bool_t);
            if(!setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (tt_char_t*)&enable, sizeof(enable))) ok = tt_true;

        }
        break;
    case TT_SOCKET_CTRL_GET_TCP_NODELAY:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_SET_RECV_BUFF_SIZE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_GET_RECV_BUFF_SIZE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_SET_SEND_BUFF_SIZE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_GET_SEND_BUFF_SIZE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_SET_NOSIGPIPE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_SET_KEEPALIVE:
        {
            tt_trace_noimpl();
        }
        break;
    case TT_SOCKET_CTRL_SET_TCP_KEEPALIVE:
        {
            tt_trace_noimpl();
        }
        break;
    default:
        tt_trace_e("unkonw socket ctrl, %lu", ctrl);
        break;
    }

    // exit args
    va_end(args);

    // ok?
    return ok;
}

tt_long_t tt_socket_connect(tt_socket_ref_t sock, tt_ipaddr_ref_t addr)
{
    // check
    tt_assert_and_check_return_val(sock && addr, -1);
    tt_assert_and_check_return_val(!tt_ipaddr_is_empty(addr), -1);

    // load addr
    tt_int_t                n = 0;
    struct sockaddr_storage d = {0};
    if(!(n = tt_sockaddr_load(&d, addr))) return -1;

    // connect
    tt_long_t r = connect(tt_socket2fd(sock), (struct sockaddr *)&d, n);

    // trace
    tt_trace_d("connect, %p, %ld, error, %d", sock, r, errno);

    // ok?
    if(!r || errno == EISCONN) return 1;

    // continue?
    if(errno == EINTR || errno == EINPROGRESS || errno == EAGAIN) return 0;

    // error
    return -1;
}

tt_bool_t tt_socket_bind(tt_socket_ref_t sock, tt_ipaddr_ref_t addr)
{
    //check
    tt_assert_and_check_return_val(sock && addr, tt_false);

    // load addr
    tt_int_t                n = 0;
    struct sockaddr_storage d = {0};
    if(!(n = tt_sockaddr_load(&d, addr))) return tt_false;

    // reuse addr
#ifdef SO_REUSEADDR
    {
        tt_int_t reuseaddr = 1;
        if(setsockopt(tt_socket2fd(sock), SOL_SOCKET, SO_REUSEADDR, (tt_int_t *)&reuseaddr, sizeof(reuseaddr)) < 0)
        {
            tt_trace_e("reuseaddr: failed");
        }
    }
#endif

#ifdef SO_REUSEPORT
    if(tt_ipaddr_port(addr))
    {
        tt_int_t reuseport = 1;
        if(setsockopt(tt_socket2fd(sock), SOL_SOCKET, SO_REUSEPORT, (tt_int_t *)&reuseport, sizeof(reuseport)) < 0)
        {
            tt_trace_e("reuseport: %u failed", tt_ipaddr_port(addr));
        } 
    }
#endif

    // only bind ipv6 address
    if(tt_ipaddr_family(addr) == TT_IPADDR_FAMILY_IPV6)
    {
        tt_int_t only_ipv6 = 1;
        if(setsockopt(tt_socket2fd(sock), IPPROTO_IPV6, IPV6_V6ONLY, (tt_int_t *)&only_ipv6, sizeof(only_ipv6)) < 0)
        {
            tt_trace_e("set only ipv6 failed");
        }
    }

    // bind
    return !bind(tt_socket2fd(sock), (struct sockaddr *)&d, n);
}

tt_bool_t tt_socket_listen(tt_socket_ref_t sock, tt_size_t backlog)
{
    // check
    tt_assert_and_check_return_val(sock, tt_false);

    // listen
    return (listen(tt_socket2fd(sock), backlog) < 0) ? tt_false : tt_true;
}

tt_socket_ref_t tt_socket_accept(tt_socket_ref_t sock, tt_ipaddr_ref_t addr)
{
    // check
    tt_assert_and_check_return_val(sock, tt_null);

    // done
    struct sockaddr_storage d = {0};
    socklen_t               n = sizeof(struct sockaddr_in);
    tt_long_t               fd = accept(tt_socket2fd(sock), (struct sockaddr *)&d, &n);

    // no client?
    tt_check_return_val(fd > 0, tt_null);

    // set non-block
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);

    /* disable the nagle's algorithm to fix 40ms ack delay in some case (.e.g send-send-40ms-recv)
     *
     * 40ms is the tcp ack delay on linux, which indicates that you are likely 
     * encountering a bad interaction between delayed acks and the nagle's algorithm. 
     *
     * the best way to address this is to send all of your data using a single call to 
     * send() or sendmsg(), before waiting for a response. 
     *
     * if that is not possible then certain tcp socket options including TCP_QUICKACK (on the receiving side), 
     * TCP_CORK (sending side), and TCP_NODELAY (sending side) can help, 
     * but can also hurt if used improperly.  
     *
     * TCP_NODELAY simply disables the nagle's algorithm and is a one-time setting on the socket, 
     * whereas the other two must be set at the appropriate times during the life of the connection 
     * and can therefore be trickier to use.
     * 
     * so we set TCP_NODELAY to reduce response delay for the accepted socket in the server by default
     */
    tt_int_t enable = 1;
    setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, (tt_char_t *)&enable, sizeof(enable));

    // save address
    if(addr) tt_sockaddr_save(addr, &d);

    // ok
    return tt_fd2socket(fd);
}

tt_bool_t tt_socket_local(tt_socket_ref_t sock, tt_ipaddr_ref_t addr)
{
    //check
    tt_assert_and_check_return_val(sock, tt_false);

    // get local address
    struct sockaddr_storage d = {0};
    tt_int_t                n = sizeof(d);
    if(getsockname(tt_socket2fd(sock), (struct sockaddr *)&d, (socklen_t *)&n) == -1) return tt_false;

    // save address
    if (addr) tt_sockaddr_save(addr, &d);

    // ok
    return tt_true;
}

tt_bool_t tt_socket_kill(tt_socket_ref_t sock, tt_size_t mode)
{
    // check
    tt_assert_and_check_return_val(sock, tt_false);

    // init how
    tt_int_t how = SHUT_RDWR;
    switch (mode)
    {
    case TT_SOCKET_KILL_RO:
        how = SHUT_RD;
        break;
    case TT_SOCKET_KILL_WO:
        how = SHUT_WR;
        break;
    case TT_SOCKET_KILL_RW:
        how = SHUT_RDWR;
        break;
    default:
        break;
    }

    // kill it
    tt_bool_t ok = (!shutdown(tt_socket2fd(sock), how))? tt_true : tt_false;

    // failed
    if (!ok) tt_trace_e("kill: %p failed, errno: %d", sock, errno);

    // ok?
    return ok;
}

tt_bool_t tt_socket_exit(tt_socket_ref_t sock)
{
    // check
    tt_assert_and_check_return_val(sock, tt_null);

    // trace 
    tt_trace_d("close: %p", sock);

    // close it
    tt_bool_t ok = (!close(tt_socket2fd(sock)))? tt_true : tt_false;

    // failed?
    if(!ok) tt_trace_e("close: %p failed, errno: %d", sock, errno);
    
    // ok?
    return ok;
}

tt_long_t tt_socket_recv(tt_socket_ref_t sock, tt_byte_t* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(sock && data, -1);
    tt_check_return_val(size, 0);

    // recv
    tt_long_t real = recv(tt_socket2fd(sock), data, (tt_int_t)size, 0);

    // trace
    tt_trace_d("recv: %p %lu => %ld, errno, %d", sock, size, real, errno);

    // ok?
    if(read >= 0) return real;

    // error
    return -1;
}

tt_long_t tt_socket_send(tt_socket_ref_t sock, tt_byte_t const* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(sock && data, -1);
    tt_check_return_val(size , 0);

    // send
    tt_long_t real = send(tt_socket2fd(sock), data, (tt_int_t)size, 0);

    // trace
    tt_trace_d("send: %p %lu => %ld, errno, %d", sock, size, real, errno);

    // ok?
    if(read >= 0) return real;

    // error
    return -1;
}

#if 0
tt_hong_t tt_socket_sendf(tt_socket_ref_t sock, tt_file_ref_t file, tt_hize_t offset, tt_hize_t size)
{
    // check
    tt_assert_and_check_return_val(sock && file && size, -1);

    // send it
    off_t       seek = offset;
    tt_hong_t   real = sendfile(tt_socket2fd(sock), tt_socket2fd(file), &seek, (size_t)size);

    // ok?
    if(real >= 0) return real;

    // continue?
    if(errno == EINTR || errno == EAGAIN) return 0;

    // error
    return -1;
}
#endif

tt_long_t tt_socket_urecv(tt_socket_ref_t sock, tt_ipaddr_ref_t addr, tt_byte_t* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(sock && data, -1);

    // no size?
    tt_check_return_val(size, 0);

    // recv
    struct sockaddr_storage d = {0};
    socklen_t               n = sizeof(d);
    tt_long_t               r = recvfrom(tt_socket2fd(sock), data, (tt_int_t)size, 0, (struct sockaddr *)&d, &n);

    // trace
    tt_trace_d("urecv: %p %lu bytes => %ld bytes, errno, %d", sock, size, r, errno);

    // ok/
    if(r >= 0)
    {
        // save address
        if(addr) tt_sockaddr_save(addr, &d);

        // ok
        return r;
    }

    // continue?
    if(errno == EINTR || errno == EAGAIN) return 0;

    // error
    return -1;
}

tt_long_t tt_socket_usend(tt_socket_ref_t sock, tt_ipaddr_ref_t addr, tt_byte_t const* data, tt_size_t size)
{
    // check
    tt_assert_and_check_return_val(sock && addr && data, -1);
    tt_assert_and_check_return_val(!tt_ipaddr_is_empty(addr), -1);

    // no size?
    tt_check_return_val(size, 0);

    // load addr
    tt_size_t               n = 0;
    struct sockaddr_storage d = {0};
    if(!(n = tt_sockaddr_load(&d, addr))) return -1;

    // send
    tt_long_t r = sendto(tt_socket2fd(sock), data, (tt_int_t)size, 0, (struct sockaddr*)&d, n);

    // trace
    tt_trace_d("usend: %p %lu bytes => %ld bytes, errno, %d", sock, size, r, errno);

    // ok/
    if(r >= 0) return r;

    // continue?
    if(errno == EINTR || errno == EAGAIN) return 0;

    // error
    return -1;
}

tt_long_t tt_socket_wait(tt_socket_ref_t sock, tt_size_t events, tt_long_t timeout)
{
    return tt_socket_wait_impl(sock, events, timeout);
}

tt_bool_t tt_socket_brecv(tt_socket_ref_t sock, tt_byte_t* data, tt_size_t size)
{
    // recv data
    tt_size_t recv = 0;
    tt_long_t wait = 0;

    while(recv < size)
    {
        // recv it
        tt_long_t real = tt_socket_recv(sock, data + recv, size - recv);

        // has data
        if(real > 0)
        {
            recv += real;
            wait  = 0;
        }
        else if(!real && !wait)
        {
            // wait it
            wait = tt_socket_wait(sock, TT_SOCKET_EVENT_RECV, -1);
            tt_check_break(wait > 0);
        }
        // failed or end?
        else break;
    }

    return recv == size;
}

tt_bool_t tt_socket_bsend(tt_socket_ref_t sock, tt_byte_t const* data, tt_size_t size)
{
    // send data
    tt_size_t send = 0;
    tt_long_t wait = 0;

    while(send < size)
    {
        // recv it
        tt_long_t real = tt_socket_send(sock, data + send, size - send);

        // has data
        if(real > 0)
        {
            send += real;
            wait  = 0;
        }
        else if(!real && !wait)
        {
            // wait it
            wait = tt_socket_wait(sock, TT_SOCKET_EVENT_SEND, -1);
            tt_check_break(wait > 0);
        }
        // failed or end?
        else break;
    }

    return send == size;
}

