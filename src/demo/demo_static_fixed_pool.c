/* //////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "ttlib.h"

tt_byte_t g_buffer[1000] = {0};

tt_void_t tt_demo_static_fixed_pool(tt_void_t)
{
	tt_static_fixed_pool_ref_t pool = tt_null;

	// trace g_buffer
	tt_trace_d("g_buffer,%p", g_buffer);
#if 0
	// noraml 
	do{
		// init pool
		pool = tt_static_fixed_pool_init(g_buffer, sizeof(g_buffer), 8, tt_false);
		tt_assert_and_check_break(pool);

		// malloc it
		tt_byte_t* data = tt_static_fixed_pool_malloc(pool __tt_debug_val__);

		// free it
		tt_bool_t ok = tt_static_fixed_pool_free(pool, data __tt_debug_val__);

	    if (pool) tt_static_fixed_pool_exit(pool);
		
	} while (0);
#endif

#if 1
	// do malloc times > manx 
	do{
		tt_trace_d("----------------------");
		// init pool
		pool = tt_static_fixed_pool_init(g_buffer, sizeof(g_buffer), 8, tt_false);
		tt_assert_and_check_break(pool);

		// manx
		tt_size_t i    = 0;
	    tt_size_t maxn = tt_static_fixed_pool_maxn(pool);
		tt_byte_t* data[1000] = {0};

		// malloc it
		for(i = 0; i < 31; i++)
		{
			data[i] = tt_static_fixed_pool_malloc(pool __tt_debug_val__);
			tt_trace_d("%d, data,%p", i, data[i]);	
		}

		for(i = 31; i < maxn; i++)
		{
			data[i] = tt_static_fixed_pool_malloc(pool __tt_debug_val__);
			tt_trace_d("%d, data,%p", i, data[i]);	
		}

		// malloc error
		// data[i] = tt_static_fixed_pool_malloc(pool __tt_debug_val__);
		// tt_trace_d("%d, data,%p", i, data[i]);	

		// free it
		for(i = 0; i < maxn; i++)
		{
			tt_trace_d("%d, data,%p", i, data[i]);	
			tt_static_fixed_pool_free(pool, data[i] __tt_debug_val__);
		}
		// free error
		tt_static_fixed_pool_free(pool, data[i] __tt_debug_val__);

	    if (pool) tt_static_fixed_pool_exit(pool);
		
	} while (0);
#endif
	// exit pool
	if (pool) tt_static_fixed_pool_exit(pool);
}
