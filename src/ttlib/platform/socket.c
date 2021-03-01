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
 * implementation
 */
