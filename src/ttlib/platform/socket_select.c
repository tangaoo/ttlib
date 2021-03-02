/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       socket_select.c
 * @ingroup    platfrom
 * @author     tango
 * @date       2021-03-02 
 * @brief      socket_select.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "socket.h"
#include <sys/socket.h>
#include <sys/select.h>
#include <errno.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_long_t tt_socket_wait_impl(tt_socket_ref_t sock, tt_size_t events, tt_long_t timeout)
{
    // check
    tt_assert_and_check_return_val(sock, -1);

    // fd
    tt_long_t fd = tt_socket2fd(sock);
    tt_assert_and_check_return_val(fd >= 0, -1);

    // init time
    struct timeval t = {0};
    if(timeout > 0)
    {
        t.tv_sec  = timeout / 1000;
        t.tv_usec = timeout % 1000 * 1000;
    }

    // init fds
    fd_set  rfds;
    fd_set  wfds;
    fd_set* prfds = (events & TT_SOCKET_EVENT_RECV)? &rfds : tt_null;
    fd_set* pwfds = (events & TT_SOCKET_EVENT_SEND)? &wfds : tt_null;

    if(prfds)
    {
        FD_ZERO(prfds);
        FD_SET(fd, prfds);
    }

    if(pwfds)
    {
        FD_ZERO(pwfds);
        FD_SET(fd, pwfds);
    }

    tt_long_t r = select(fd + 1, prfds, pwfds, tt_null, timeout >= 0? &t : tt_null);
    // timeout or interrupted?
    if(!r || (r == -1 && errno == EINTR)) return 0;

    // check
    tt_assert_and_check_return_val(r >= 0, -1);

    // check socket error
    tt_int_t error = 0;
    socklen_t    n = sizeof(socklen_t);
    if(!getsockopt(fd, SOL_SOCKET, SO_ERROR, (tt_char_t*)&error, &n) && error) // TODO
        return -1;
    
    // ok
    tt_long_t e = TT_SOCKET_EVENT_NONE;
    if(prfds && FD_ISSET(fd, &rfds)) e |= TT_SOCKET_EVENT_RECV;
    if(pwfds && FD_ISSET(fd, &wfds)) e |= TT_SOCKET_EVENT_SEND;

    return e; 
}