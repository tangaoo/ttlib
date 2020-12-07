/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       main.c
 * @ingroup    main
 * @author     tango
 * @date       2020-10 
 * @brief      main.c file
 */

#define TT_TRACE_MODULE_NAME     "MAIN"
#define TT_TRACE_DEBUG           (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include <stdlib.h>
#include <stdio.h>
#include "ttlib.h"
#include "demo.h"


tt_int_t main(tt_int_t argc, tt_char_t** argv)
{
#if 1
	if (!tt_lib_init(tt_null, tt_native_allocator())) return -1;
#else
	if (!tt_init(tt_null, tt_static_large_allocator_init((tb_byte_t*)malloc(1 * 1024 * 1024), 1 * 1024 * 1024, 8))) return -1;
#endif		
	
	tt_demo_utils_dump_main(argc, argv);

	system("pause"); //stop
	return 0;
}
