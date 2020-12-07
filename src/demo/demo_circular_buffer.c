/* //////////////////////////////////////////////////////////////////////////////////////
* define
*/
#define TT_TRACE_MODULE_NAME          "DEMO_CB"
#define TT_TRACE_MODULE_DEBUG         (1)

#include "ttlib.h" 
#include <string.h>

#define BUFF_SIZE        8

static tt_circular_buffer_t   s_cb;
static tt_byte_t              buffer[BUFF_SIZE];
static tt_byte_t              in[] = "123456";
static tt_byte_t              out[BUFF_SIZE];
static tt_byte_t              cover[] = "0123456789";

tt_void_t demo_circular_buffer_full(tt_void_t)
{
	/// clear buffer
	memset(buffer, 0, sizeof(buffer));

	tt_circular_buffer_init(&s_cb, buffer, sizeof(buffer));

	/// write full
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));

	tt_circular_buffer_exit(&s_cb);
}

tt_void_t demo_circular_buffer_empty(tt_void_t)
{
	/// clear buffer
	memset(buffer, 0, sizeof(buffer));

	tt_circular_buffer_init(&s_cb, buffer, sizeof(buffer));

	/// write full
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));
	tt_circular_buffer_writ(&s_cb, in, strlen((tt_char_t*)in));

	/// read empty
	tt_circular_buffer_read(&s_cb, out, sizeof(out));
	tt_circular_buffer_read(&s_cb, out, sizeof(out));
	tt_circular_buffer_read(&s_cb, out, sizeof(out));

	tt_circular_buffer_exit(&s_cb);
}

tt_void_t demo_circular_buffer_writ_slow(tt_void_t)
{
	/// clear buffer
	memset(buffer, 0, sizeof(buffer));

	tt_circular_buffer_init(&s_cb, buffer, sizeof(buffer));

	///
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);
	tt_circular_buffer_writ(&s_cb, in, 4);
	tt_circular_buffer_read(&s_cb, out, 5);

	tt_circular_buffer_exit(&s_cb);

}

tt_void_t demo_circular_buffer_read_slow(tt_void_t)
{
	/// clear buffer
	memset(buffer, 0, sizeof(buffer));

	tt_circular_buffer_init(&s_cb, buffer, sizeof(buffer));

	///
	tt_circular_buffer_writ(&s_cb, in, 6);
	tt_circular_buffer_read(&s_cb, out, 4);
	tt_circular_buffer_writ(&s_cb, in, 6);
	tt_circular_buffer_read(&s_cb, out, 4);
	tt_circular_buffer_writ(&s_cb, in, 6);
	tt_circular_buffer_read(&s_cb, out, 4);
	tt_circular_buffer_writ(&s_cb, in, 6);
	tt_circular_buffer_read(&s_cb, out, 4);

	tt_circular_buffer_exit(&s_cb);

}

tt_void_t demo_circular_buffer_write_cover(tt_void_t)
{
	memset(buffer, 0, sizeof(buffer));

	tt_circular_buffer_init(&s_cb, buffer, sizeof(buffer));
	tt_circular_buffer_writ_cover(&s_cb, (tt_byte_t*)"AAAAAAA", 7);
	tt_circular_buffer_writ_cover(&s_cb, cover, 10);

	tt_circular_buffer_exit(&s_cb);
}

tt_void_t tt_demo_circular_buffer_main(tt_void_t)
{
	demo_circular_buffer_full();
	demo_circular_buffer_empty();
	demo_circular_buffer_writ_slow();
	demo_circular_buffer_read_slow();
	demo_circular_buffer_write_cover();

}