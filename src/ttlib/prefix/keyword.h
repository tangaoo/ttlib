/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       keyword.h
 * @ingroup    prefix
 * @author     tango
 * @date       2020-11-19 
 * @brief      keyword.h file
 */

#ifndef _PREFIX_KEYWORD_H_
#define _PREFIX_KEYWORD_H_

/*//////////////////////////////////////////////////////////////////////////////////////
* includes 
*/
#include "config.h"
#include "type.h"

//extern c
#ifdef __cplusplus
#   define __tt_extern_c__                            extern "C"
#	define __tt_extern_c_enter__                      extern "C" {
#	define __tt_extern_c_leave__                      }
#else
#   define __tt_extern_c__
#	define __tt_extern_c_enter__                        
#	define __tt_extern_c_leave__
#endif

#define __tt_volatile__	                              volatile
#define __tt_func__                                   __FUNCTION__
#define __tt_file__                                   __FILE__
#define __tt_line__                                   __LINE__
#define __tt_new_line__                               "\n"
#define __tt_inline__                                 __inline

#if __tt_debug__
#	define __tt_debug_decl__                          , tt_char_t const* func_, tt_uint32_t line_, tt_char_t const* file_
#	define __tt_debug_args__                          , func_, line_, file_
#	define __tt_debug_val__                           , __tt_func__, __tt_line__, __tt_file__
#else
#	define __tt_debug_decl__                          
#	define __tt_debug_args__                          
#	define __tt_debug_val__ 
#endif

#if TT_COMPILER_IS_GCC
#   define __tt_aligned__(a)                    __attribute__((aligned(a)))
#elif TT_COMPILER_IS_MSVC
#   define __tt_aligned__(a)                    __declspec(align(a))
#endif

#endif