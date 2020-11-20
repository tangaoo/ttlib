
/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       ttlib.c
 * @ingroup    ttlib
 * @author     tango
 * @date       2020-11-19 
 * @brief      ttlib.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"
#include "memory/impl/impl.h"

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

		tt_trace_d("ttlib init success!");
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