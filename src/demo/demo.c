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
#include <string.h>
#include "ttlib.h"
#include "demo.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define TT_DEMO_MAIN_ITEM(name)          {#name, tt_demo_##name##_main}

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the demo type
typedef struct __tt_demo_t
{
	// demo name
	tt_char_t const*   name;
	
	// the demo main func
	tt_int_t           (*main)(tt_int_t argc, tt_char_t** argv);

}tt_demo_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */

// the demo array
static tt_demo_t g_demo[] = {
	// demo
	TT_DEMO_MAIN_ITEM(utils),
	TT_DEMO_MAIN_ITEM(utils_dump),
	TT_DEMO_MAIN_ITEM(utils_mix),
	TT_DEMO_MAIN_ITEM(circular_buffer),
	TT_DEMO_MAIN_ITEM(single_list_entry),
	TT_DEMO_MAIN_ITEM(crc8),
	TT_DEMO_MAIN_ITEM(queue_entry),
	TT_DEMO_MAIN_ITEM(static_fixed_pool),

};

tt_int_t main(tt_int_t argc, tt_char_t** argv)
{
#if 1
	if (!tt_lib_init(tt_null, tt_native_allocator())) return -1;
#else
#endif		
	
	// find the demo main
	tt_int_t         ok;
	tt_char_t const* name = tt_null;

	if(argc > 1 && argv[1])
	{
		tt_size_t i;
		tt_size_t n = tt_arrayn(g_demo);
		for(i = 0; i < n; i++)
		{
			if(g_demo[i].name && !strcmp(g_demo[i].name, argv[1])) 
			{
				// save name
				name = g_demo[i].name;

				// done demo 
				ok = g_demo[i].main(argc - 1, argv - 1);
				break;
			}
		}
	}

	// not find
	if(!name)
	{
		tt_size_t i;
		tt_size_t n = tt_arrayn(g_demo);

		// done all
		// for(i = 0; i < n; i++) g_demo[i].main(argc - 1, argv - 1);
 
		// print usage
		tt_trace_i("%s", "---------------------------------------");
		tt_trace_i("|  please check demo name!            |");	
		tt_trace_i("|  usage, xmake r ttlib_demo utils    |");
		tt_trace_i("%s", "---------------------------------------");
		// walk and prinf
		for(i = 0; i < n; i++) tt_trace_i("%s", g_demo[i].name);
	}

	// exit
	tt_lib_exit();
	return 0;
}
