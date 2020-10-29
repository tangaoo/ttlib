#ifndef _TT_PREFIC_CONFIG_
#define _TT_PREFIC_CONFIG_

/// c++
#ifdef __cplusplus
#undef __cplusplus
#endif

/// debug
#define __tt_debug__                   1

/// platform
#if 1
#	define TT_WIN_MSVC
#elif 0
#	define TT_DSP
#elif 0
#	define TT_ARM
#endif

/// 64 bit
#define TT_CPU_BIT_64               1

/// gcc complie
#define TT_COMPILER_IS_GCC          0

#endif