
/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       ttlib.c
 * @ingroup    ttlib
 * @author     tango
 * @date       2020-11-19 
 * @brief      ttlib.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace macro
 */
#define TT_TRACE_MODULE_NAME     "ttlib"
#define TT_TRACE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"
#include "memory/impl/impl.h"

// the ttlib version
static const char* version = "v1.1.1";

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_bool_t tt_lib_init(tt_handle_t priv, tt_allocator_ref_t allocator)
{
	tt_bool_t ret = tt_false;
	
	/// done
	do{
		/// trace init
		if(!tt_trace_init()) break;

		/// the memory init
		if(!tt_memory_env_init(allocator)) break;

		tt_trace_d("ttlib(%s) init success!", version);

		// detect compiler and print
#ifdef TT_COMPILER_IS_GCC
		tt_trace_d("compiler is gcc", version);
#elif defined TT_COMPILER_IS_MSVC 
		tt_trace_d("compiler is msvc", version);
#elif defined TT_COMPILER_IS_CLANG
		tt_trace_d("compiler is clang", version);
#elif defined TT_COMPILER_IS_DSP
		tt_trace_d("compiler is dsp", version);
#else
		tt_trace_d("compiler is unkonwn", version);
#endif

		// detect cpu endian
		if(tt_little_endian() == TT_LITTLE_ENDIAN) tt_trace_d("litter endian\n");
		else if(tt_little_endian() == TT_BIG_ENDIAN) tt_trace_d("big endian\n");
		else tt_trace_d("wrong endian\n");

		ret = tt_true;
	} while (0);
	
	if(!ret) tt_trace_e("ttlib init error!!!");

	return ret;
}

tt_void_t tt_lib_exit(tt_void_t)
{
	/// trace
	tt_trace_exit();
}