/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file demo_utils.c
 * @ingroup demo_utils
 * @author tango
 * @date 2020-10 
 * @brief demo_utils.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
* define
*/
#define TT_TRACE_MODULE_NAME          "DEMO_UTILS"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "../ttlib/ttlib.h" ///need opt

tt_uint32_t demo_utils_main(tt_void_t)
{
	tt_trace_d("***********demo_utils***********");

	tt_trace_d("-1 abs, %d", tt_abs(-1));
	tt_trace_d("max(1, 2), %d", tt_max(1, 2));
	tt_trace_d("min(1, 2), %d", tt_min(1, 2));
	tt_trace_d("max3(1, 2, 3), %d", tt_max3(1, 2, 3));
	tt_trace_d("min3(1, 2, 3), %d", tt_min3(1, 2, 3));
	int a[10];
//	tt_assert(a);
	tt_trace_d("tb_arrayn, %d", tt_arrayn(a));
	tt_trace_d("ispow2, %d, %d", tt_ispow2(4), tt_ispow2(3));
	tt_trace_d("align2, %d", tt_align2(1));
	tt_trace_d("align4, %d", tt_align4(1));
	tt_trace_d("align8, %d", tt_align8(1));
	tt_trace_d("tb_align, %d", tt_align(1, 8));
	tt_trace_d("tb_align, %d", tt_align_u32(1, 8));
	tt_trace_d("tb_align, %d", tt_align_u64(1, 8));
//  tt_trace_d("tb_align_pow2, %d", tt_align_pow2(3));

	typedef struct _test{
		int a;
		int b;
	}test_t;
	test_t test1;

	tt_trace_d("*************test check_return*************");
	//	tb_assert_and_check_abort(0);


	tt_trace_d("offsetof, %d", tt_offsetof(test_t, b));
	tt_trace_d("container, %x, b, %x", tt_container_of(test_t, b, &test1.b), &test1.b);
	tt_trace_d("tb_memsizeof, %d", tt_memsizeof(test_t, b));
	tt_trace_d("tb_memtailof, %d", tt_memtailof(test_t, b));

	tt_uint32_t x = 1, y = 2;
	tt_swap(tt_uint32_t, x, y);
	tt_trace_d("x,%d, y,%d", x, y);


	return tt_true;
}

tt_void_t demo_crc8_main(tt_void_t)
{
	tt_uint8_t crcVal;
	tt_char_t *str = "hello, world";

	crcVal = tt_crc8_make_from_cstr(str, 1);
	tt_trace_d("crc, %d", crcVal);

	crcVal = tt_crc8_make_from_cstr(str, 1);
	tt_trace_d("crc, %d", crcVal);

	crcVal = tt_crc8_make_from_cstr(str, 2);
	tt_trace_d("crc, %d", crcVal);

	crcVal = tt_crc8_make_from_cstr(str, 2);
	tt_trace_d("crc, %d", crcVal);
}