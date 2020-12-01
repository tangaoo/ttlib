/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       type.h
 * @ingroup    prefix
 * @author     tango
 * @date       2020-11-19 
 * @brief      type.h file
 */

#ifndef TT_PREFIX_TYPE_H
#define TT_PREFIX_TYPE_H

/*//////////////////////////////////////////////////////////////////////////////////////
* includes 
*/
#include <stdio.h>
#include "config.h"
#include "keyword.h"

/*//////////////////////////////////////////////////////////////////////////////////////
* macros
*/
#define tt_true                                    ((tt_bool_t)1)
#define tt_false                                   ((tt_bool_t)0)

//null
#ifdef __cplusplus
#	define tt_null                                 (0)                
#else
#	define tt_null                                 ((tt_pointer_t)0)
#endif

//argument
#ifdef __cplusplus
#	define tt_noarg_t
#else
#	define tt_noarg_t                              tt_void_t
#endif

#if defined TT_WIN_MSVC
/*//////////////////////////////////////////////////////////////////////////////////////
* type
*/
typedef signed int                                 tt_int_t;
typedef unsigned int                               tt_uint_t;
typedef signed short                               tt_short_t;
typedef unsigned short                             tt_ushort_t;
typedef signed char                                tt_int8_t;
typedef unsigned char                              tt_uint8_t;
typedef signed short                               tt_int16_t;
typedef unsigned short                             tt_uint16_t;
typedef signed int                                 tt_int32_t;
typedef unsigned int                               tt_uint32_t;
typedef signed long long                           tt_int64_t;
typedef unsigned long long                         tt_uint64_t;
typedef float                                      tt_f32_t;
typedef double                                     tt_f64_t;
typedef char                                       tt_char_t;
typedef tt_uint8_t                                 tt_byte_t;
typedef tt_int8_t                                  tt_bool_t;
typedef void                                       tt_void_t;
typedef tt_void_t *                                tt_pointer_t;
typedef tt_void_t const *                          tt_cpointer_t;
typedef tt_pointer_t                               tt_handle_t;

/// long and size
#	if defined(TT_WIN_MSVC) && TT_CPU_BIT_64
typedef tt_int64_t                                 tt_long_t;
typedef tt_uint64_t                                tt_ulong_t;
#	else
typedef signed long                                tt_long_t;
typedef unsigned long                              tt_ulong_t;
#	endif
typedef tt_ulong_t                                 tt_size_t;



#elif (defined TT_DSP)

typedef signed char                                tt_int8_t;
typedef unsigned char                              tt_uint8_t;
typedef signed short                               tt_int16_t;
typedef unsigned short                             tt_uint16_t;
typedef signed int                                 tt_int32_t;
typedef unsigned int                               tt_uint32_t;
typedef signed long long                           tt_int64_t;
typedef unsigned long long                         tt_uint64_t;
typedef float                                      tt_f32_t;
typedef double                                     tt_f64_t;
typedef char                                       tt_char_t;
typedef tt_uint8_t                                 tt_byte_t;
typedef tt_int8_t                                  tt_bool_t;
typedef void                                       tt_void_t;
typedef tt_void_t *                                tt_pointer_t;
typedef tt_void_t const *                          tt_cpointer_t;
typedef tt_pointer_t                               tt_handle_t;
typedef tt_uint32_t                                tt_size_t;

#endif

#endif