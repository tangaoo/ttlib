/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       demo_static_large_allocator.c
 * @ingroup    demo_memory
 * @author     tango
 * @date       2021-01-04 
 * @brief      demo_static_large_allocator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME          "DEMO_STATIC_LARGE_ALLOCATOR"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"
#include "../color.h"

#include <stdlib.h>

tt_void_t tt_demo_static_large_allocator_main(tt_void_t)
{ 
	// print title
	tt_print_title("demo static large allocator");

    // init large allocator
    tt_allocator_ref_t allocator = tt_static_large_allocator_init((tt_byte_t*)malloc(10 * 1024 * 1024), 10 * 1024 * 1024, 0);

    // malloc
    tt_size_t  real;
    tt_byte_t* data1 = tt_allocator_large_malloc(allocator, 10 * 1024, &real);
    tt_trace_d("data,%p, real,%d", data1, real);

    // free
    tt_allocator_large_free(allocator, data1);

    // do again
    tt_byte_t* data2 = tt_allocator_large_malloc(allocator, 10 * 1024, &real);
    tt_trace_d("data,%p, real,%d", data2, real);

    // free
    tt_allocator_large_free(allocator, data2);

}