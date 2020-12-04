/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       config.h
 * @ingroup    prefix
 * @author     tango
 * @date       2020-11-19 
 * @brief      config.h file
 */

#ifndef _TT_PREFIC_CONFIG_
#define _TT_PREFIC_CONFIG_

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

/// c++
#ifdef __cplusplus
#undef __cplusplus
#endif

/// debug
#define __tt_debug__           1

/// platform
#if 1
#	define TT_WIN_MSVC
#elif 0
#	define TT_DSP
#elif 0
#	define TT_ARM
#endif

/// gcc complie
#define TT_COMPILER_IS_GCC      1 
#define TT_COMPILER_IS_MSVC     0 

/// endian
//#define TT_WORD_BIGENDIAN
#define TT_WORD_LITTLEENDIAN


#endif