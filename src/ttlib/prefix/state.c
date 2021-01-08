/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       state.c
 * @ingroup    prefix
 * @author     tango
 * @date       2021-01-08 
 * @brief      state.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "state.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_char_t const* tt_state_cstr(tt_size_t state)
{
    // done
    switch (state)
    {
    case TT_STATE_OK:                           return "ok";
    case TT_STATE_END:                          return "end";
    case TT_STATE_DEAD:                         return "dead";
    case TT_STATE_READY:                        return "ready";
    case TT_STATE_FAILED:                       return "failed";
    case TT_STATE_KILLED:                       return "killed";
    case TT_STATE_PAUSED:                       return "paused";
    case TT_STATE_CLOSED:                       return "closed";
    case TT_STATE_OPENED:                       return "opened";
    case TT_STATE_OPENING:                      return "opening";
    case TT_STATE_SUSPEND:                      return "suspend";
    case TT_STATE_RUNNING:                      return "running";
    case TT_STATE_PENDING:                      return "pending";
    case TT_STATE_WAITING:                      return "waiting";
    case TT_STATE_WORKING:                      return "working";
    case TT_STATE_KILLING:                      return "killing";
    case TT_STATE_EXITING:                      return "exiting";
    case TT_STATE_PAUSING:                      return "pausing";
    case TT_STATE_TIMEOUT:                      return "timeout";
    case TT_STATE_FINISHED:                     return "finished";
    case TT_STATE_WAIT_FAILED:                  return "wait failed";
    case TT_STATE_POST_FAILED:                  return "post failed";
    case TT_STATE_NOT_SUPPORTED:                return "not supported";
    case TT_STATE_UNKNOWN_ERROR:                return "unknown error";

    case TT_STATE_SOCK_DNS_FAILED:              return "sock: dns: failed";
    case TT_STATE_SOCK_CONNECT_FAILED:          return "sock: connect: failed";
    case TT_STATE_SOCK_CONNECT_TIMEOUT:         return "sock: connect: timeout";
    case TT_STATE_SOCK_RECV_TIMEOUT:            return "sock: recv: timeout";
    case TT_STATE_SOCK_SEND_TIMEOUT:            return "sock: send: timeout";
    case TT_STATE_SOCK_RECV_FAILED:             return "sock: recv: failed";
    case TT_STATE_SOCK_SEND_FAILED:             return "sock: send: failed";
    case TT_STATE_SOCK_OPEN_FAILED:             return "sock: open: failed";
    case TT_STATE_SOCK_UNKNOWN_ERROR:           return "sock: unknown error";
    case TT_STATE_SOCK_SSL_FAILED:              return "sock: ssl: failed";
    case TT_STATE_SOCK_SSL_TIMEOUT:             return "sock: ssl: timeout";
    case TT_STATE_SOCK_SSL_WANT_READ:           return "sock: ssl: want read";
    case TT_STATE_SOCK_SSL_WANT_WRIT:           return "sock: ssl: want writ";
    case TT_STATE_SOCK_SSL_WAIT_FAILED:         return "sock: ssl: wait failed";
    case TT_STATE_SOCK_SSL_READ_FAILED:         return "sock: ssl: read: failed";
    case TT_STATE_SOCK_SSL_WRIT_FAILED:         return "sock: ssl: writ: failed";
    case TT_STATE_SOCK_SSL_NOT_SUPPORTED:       return "sock: ssl: not supported";
    case TT_STATE_SOCK_SSL_UNKNOWN_ERROR:       return "sock: ssl: unknown error";

    case TT_STATE_HTTP_RESPONSE_100:            return "http: response: 100";
    case TT_STATE_HTTP_RESPONSE_101:            return "http: response: 101";
    case TT_STATE_HTTP_RESPONSE_102:            return "http: response: 102";
    case TT_STATE_HTTP_RESPONSE_200:            return "http: response: 200";
    case TT_STATE_HTTP_RESPONSE_204:            return "http: response: 204";
    case TT_STATE_HTTP_RESPONSE_300:            return "http: response: 300";
    case TT_STATE_HTTP_RESPONSE_301:            return "http: response: 301";
    case TT_STATE_HTTP_RESPONSE_302:            return "http: response: 302";
    case TT_STATE_HTTP_RESPONSE_303:            return "http: response: 303";
    case TT_STATE_HTTP_RESPONSE_304:            return "http: response: 304";
    case TT_STATE_HTTP_RESPONSE_305:            return "http: response: 305";
    case TT_STATE_HTTP_RESPONSE_306:            return "http: response: 306";
    case TT_STATE_HTTP_RESPONSE_307:            return "http: response: 307";
    case TT_STATE_HTTP_RESPONSE_400:            return "http: response: 400";
    case TT_STATE_HTTP_RESPONSE_401:            return "http: response: 401";
    case TT_STATE_HTTP_RESPONSE_402:            return "http: response: 402";
    case TT_STATE_HTTP_RESPONSE_403:            return "http: response: 403";
    case TT_STATE_HTTP_RESPONSE_404:            return "http: response: 404";
    case TT_STATE_HTTP_RESPONSE_405:            return "http: response: 405";
    case TT_STATE_HTTP_RESPONSE_406:            return "http: response: 406";
    case TT_STATE_HTTP_RESPONSE_407:            return "http: response: 407";
    case TT_STATE_HTTP_RESPONSE_408:            return "http: response: 408";
    case TT_STATE_HTTP_RESPONSE_409:            return "http: response: 409";
    case TT_STATE_HTTP_RESPONSE_410:            return "http: response: 410";
    case TT_STATE_HTTP_RESPONSE_411:            return "http: response: 411";
    case TT_STATE_HTTP_RESPONSE_412:            return "http: response: 412";
    case TT_STATE_HTTP_RESPONSE_413:            return "http: response: 413";
    case TT_STATE_HTTP_RESPONSE_414:            return "http: response: 414";
    case TT_STATE_HTTP_RESPONSE_415:            return "http: response: 415";
    case TT_STATE_HTTP_RESPONSE_416:            return "http: response: 416";
    case TT_STATE_HTTP_RESPONSE_500:            return "http: response: 500";
    case TT_STATE_HTTP_RESPONSE_501:            return "http: response: 501";
    case TT_STATE_HTTP_RESPONSE_502:            return "http: response: 502";
    case TT_STATE_HTTP_RESPONSE_503:            return "http: response: 503";
    case TT_STATE_HTTP_RESPONSE_504:            return "http: response: 504";
    case TT_STATE_HTTP_RESPONSE_505:            return "http: response: 505";
    case TT_STATE_HTTP_RESPONSE_506:            return "http: response: 506";
    case TT_STATE_HTTP_RESPONSE_507:            return "http: response: 507";
    case TT_STATE_HTTP_RESPONSE_UNK:            return "http: response: unknown code";
    case TT_STATE_HTTP_RESPONSE_NUL:            return "http: response: no";
    case TT_STATE_HTTP_REQUEST_FAILED:          return "http: request: failed";
    case TT_STATE_HTTP_REDIRECT_FAILED:         return "http: redirect: failed";
    case TT_STATE_HTTP_POST_FAILED:             return "http: post: failed";
    case TT_STATE_HTTP_RANGE_INVALID:           return "http: range: invalid";
    case TT_STATE_HTTP_GZIP_NOT_SUPPORTED:      return "http: gzip: not supported";
    case TT_STATE_HTTP_UNKNOWN_ERROR:           return "http: unknown error";

    case TT_STATE_DATABASE_NO_SUCH_TABLE:       return "database: no such table";
    case TT_STATE_DATABASE_NO_SUCH_FIELD:       return "database: no such field";
    case TT_STATE_DATABASE_NO_SUCH_DATABASE:    return "database: no such database";
    case TT_STATE_DATABASE_ACCESS_DENIED:       return "database: access denied";
    case TT_STATE_DATABASE_PARSE_ERROR:         return "database: parse error";
    case TT_STATE_DATABASE_VALUE_COUNT_ERROR:   return "database: value count error";
    case TT_STATE_DATABASE_UNKNOWN_HOST:        return "database: unknown host";
    case TT_STATE_DATABASE_UNKNOWN_ERROR:       return "database: unknown error";

    case TT_STATE_SYSERROR_NOT_PERM:            return "syserror: permission denied";
    case TT_STATE_SYSERROR_NOT_FILEDIR:         return "syserror: not file or directory";
    case TT_STATE_SYSERROR_UNKNOWN_ERROR:       return "syserror: unknown error";

    default:                                    return "unknown";
    }

    return tt_null;    
}
