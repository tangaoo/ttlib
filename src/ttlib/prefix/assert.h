#ifndef _TT_ASSERT_H_
#define _TT_ASSERT_H_

#include "abort.h"
#include "config.h"
#include "trace.h"

__tt_extern_c_enter__

#if defined(__tt_debug__)
#	define tt_assert(x)                                               do{ if(!(!!(x))) {tt_trace_d("expr [%s], %s(), %d, %s", #x __tt_debug_val__); tt_abort_done();} }while(0)
#   define tt_assert_and_check_abort(x)                               tt_assert(x) //TODO
#   define tt_assert_and_check_return(x)                              tt_assert(x)
#   define tt_assert_and_check_return_val(x, v)                       tt_assert(x)
#   define tt_assert_and_check_goto(x, b)                             tt_assert(x)
#   define tt_assert_and_check_break(x)                               tt_assert(x)
#   define tt_assert_and_check_continue(x)                            tt_assert(x)
#   define tt_assert_and_check_break_state(x, s, v)                   tt_assert(x)
#else
#	define tt_assert(x)
#   define tt_assert_and_check_abort(x)                        
#   define tt_assert_and_check_return(x)                        
#   define tt_assert_and_check_return_val(x, v)                 
#   define tt_assert_and_check_goto(x, b)                       
#   define tt_assert_and_check_break(x)                         
#   define tt_assert_and_check_continue(x)                      
#   define tt_assert_and_check_break_state(x, s, v)             
#endif

#if defined(__tt_debug__)
#   define tt_assertf(x, fmt, ...)                                    do { if (!(x)) { tt_tracef_d("expr [%s], %s(), %d, %s: " fmt, #x __tt_debug_val__, __VA_ARGS__); tt_abort_done(); } } while(0) 
#   define tt_assertf_and_check_return(x, fmt, ...)                   tt_assertf(x, fmt, __VA_ARGS__) //TODO
#   define tt_assertf_and_check_return_val(x, v, fmt, ...)            tt_assertf(x, fmt, __VA_ARGS__)
#   define tt_assertf_and_check_goto(x, b, fmt, ...)                  tt_assertf(x, fmt, __VA_ARGS__)
#   define tt_assertf_and_check_break(x, fmt, ...)                    tt_assertf(x, fmt, __VA_ARGS__)
#   define tt_assertf_and_check_continue(x, fmt, ...)                 tt_assertf(x, fmt, __VA_ARGS__)
#else
#   define tb_assertf(x, fmt, ...)                                      
#   define tb_assertf_and_check_return(x, fmt, ...)                     
#   define tb_assertf_and_check_return_val(x, v, fmt, ...)              
#   define tb_assertf_and_check_goto(x, b, fmt, ...)                    
#   define tb_assertf_and_check_break(x, fmt, ...)                      
#   define tb_assertf_and_check_continue(x, fmt, ...)                             
#endif


__tt_extern_c_enter__

#endif