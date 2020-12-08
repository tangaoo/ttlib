/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       demo_utils_dump.c
 * @ingroup    demo_utils
 * @author     tango
 * @date       2020-11-19 
 * @brief      demo_utils_dump.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
*  trace 
*/
#define TT_TRACE_MODULE_NAME          "DEMO_UTILS_DUMP"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_void_t tt_demo_utils_dump_main(tt_void_t)
{
    tt_trace_raw("\n");
    tt_trace_d("----------------------- demo_utils_dump");

    // memory_dump_test
	do{
		tt_byte_t bytes[100] = {0};

		bytes[0]  = 1;
		bytes[99] = 1;
		tt_trace_d("%p", bytes);

		tt_memory_dump(bytes, sizeof(bytes));
	}while(0);

}
