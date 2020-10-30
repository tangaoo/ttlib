#ifndef _PREFIX_TRACE_H_
#define _PREFIX_TRACE_H_

#include "../utils/trace.h"

__tt_extern_c_enter__

/// trace prefix
#ifndef __tt_prefix__
#	define __tt_prefix__                        tt_null
#endif

/// trace module name
#ifndef TT_TRACE_MODULE_NAME
#	define TT_TRACE_MODULE_NAME                 tt_null
#endif

/// trace 
#ifndef TT_TRACE_DEBUG
#	define TT_TRACE_DEBUG                       (1)
#endif

/// debug
#define	tt_trace_p(prefix, fmt, ...)            do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, fmt __tt_new_line__, __VA_ARGS__); }while(0)
#define tt_tracef_p(prefix, fmt, ...)           do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, fmt, __VA_ARGS__); }while(0)

/// for print
#define tt_trace_error_p(prefix, fmt, ...)      do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[error  ], %s(), %d: "fmt __tt_new_line__, __tt_func__, __tt_line__ ,__VA_ARGS__); }while(0)
#define tt_trace_assert_p(prefix, fmt, ...)     do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[assert ], %s(), %d: "fmt __tt_new_line__, __tt_func__, __tt_line__ ,__VA_ARGS__); }while(0)
#define tt_trace_warning_p(prefix, fmt, ...)    do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[warning], %s(), %d: "fmt __tt_new_line__, __tt_func__, __tt_line__ ,__VA_ARGS__); }while(0)

/// for file
#define tt_tracef_error_p(prefix, fmt, ...)     do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[error  ], %s(), %d, %s: "fmt, __tt_func__, __tt_line__ , __tt_file__ ,__VA_ARGS__); }while(0)
#define tt_tracef_assert_p(prefix, fmt, ...)    do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[assert ], %s(), %d, %s: "fmt, __tt_func__, __tt_line__ , __tt_file__ ,__VA_ARGS__); }while(0)
#define tt_tracef_warning_p(prefix, fmt, ...)   do{ tt_trace_done(prefix, TT_TRACE_MODULE_NAME, "[warning], %s(), %d, %s: "fmt, __tt_func__, __tt_line__ , __tt_file__ ,__VA_ARGS__); }while(0)

#if TT_TRACE_DEBUG && defined(__tt_debug__)
#	define tt_trace_d(fmt, ...)                 tt_trace_p(__tt_prefix__, fmt, __VA_ARGS__) 
#	define tt_tracef_d(fmt, ...)                tt_tracef_p(__tt_prefix__, fmt, __VA_ARGS__) 
#else
#	define tt_trace_d(fmt, ...)
#endif

#define tt_trace_i(fmt, ...)                    tt_trace_p(__tt_prefix__, fmt, __VA_ARGS__)
#define tt_trace_w(fmt, ...)                    tt_trace_warning_p(__tt_prefix__, fmt, __VA_ARGS__)
#define tt_trace_a(fmt, ...)                    tt_trace_assert_p(__tt_prefix__, fmt, __VA_ARGS__)
#define tt_trace_e(fmt, ...)                    tt_trace_error_p(__tt_prefix__, fmt, __VA_ARGS__)

/// raw
 #define tt_trace_raw(fmt, ...)                  do{ tt_trace_done(tt_null, tt_null, fmt __tt_new_line__, __VA_ARGS__); }while(0)


__tt_extern_c_leave__

#endif