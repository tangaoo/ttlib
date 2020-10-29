/* //////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "../ttlib/ttlib.h" ///need opt

#if 0

tb_void_t tb_demo_static_fixed_pool_perf(tb_size_t item_size)
{
	// done
	tb_static_fixed_pool_ref_t pool = tb_null;
	do
	{
		// init pool
		pool = tb_static_fixed_pool_init((tb_byte_t*)malloc(1 * 1024 * 1024), 1 * 1024 * 1024, item_size, tb_false);
		tb_assert_and_check_break(pool);

		// make data list
		tb_size_t       maxn = 10;
		tb_pointer_t*   list = (tb_pointer_t*)calloc(maxn, sizeof(tb_pointer_t));
		tb_assert_and_check_break(list);

		// done 
		__tb_volatile__ tb_size_t indx = 0;
//		__tb_volatile__ tb_hong_t time = tb_mclock();
		__tb_volatile__ tb_size_t rand = 0xbeaf;
		for (indx = 0; indx < maxn; indx++)
		{
			// make data
			list[indx] = tb_static_fixed_pool_malloc(pool __tb_debug_vals__);
			tb_assert_and_check_break(list[indx]);

			// make rand
			rand = (rand * 10807 + 1) & 0xffffffff;

			// free data
			__tb_volatile__ tb_size_t size = rand & 15;
			if (size > 5 && indx)
			{
				size -= 5;
				while (size--)
				{
					// the free index
					tb_size_t free_indx = rand % indx;

					// free it
					if (list[free_indx]) tb_static_fixed_pool_free(pool, list[free_indx] __tb_debug_vals__);
					list[free_indx] = tb_null;
				}
			}
		}
//		time = tb_mclock() - time;

#ifdef __tb_debug__
		// dump pool
		tb_static_fixed_pool_dump(pool);
#endif

		// trace
//		tb_trace_i("time: %lld ms", time);

		// clear pool
		tb_static_fixed_pool_clear(pool);

	} while (0);

	// exit pool
	if (pool) tb_static_fixed_pool_exit(pool);
}

#endif