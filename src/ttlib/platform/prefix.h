/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       prefix.h
 * @ingroup    platform
 * @author     tango
 * @date       2020-11-19 
 * @brief      prefix.h file
 */

#ifndef TT_PLATFORM_PREFIX_H
#define TT_PLATFORM_PREFIX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// fd to file
#define tt_fd2file(fd)              ((fd) >= 0? (tt_file_ref_t)((tt_long_t)(fd) + 1) : tt_null)

// file to fd
#define tt_file2fd(file)            (tt_int_t)((file)? (((tt_long_t)(file)) - 1) : -1)

// fd to sock
#define tt_fd2socket(fd)            ((fd) >= 0? (tt_socket_ref_t)((tt_long_t)(fd) + 1) : tt_null)

// sock to fd
#define tt_socket2fd(sock)          (tt_int_t)((sock)? (((tt_long_t)(sock)) - 1) : -1)


#endif
