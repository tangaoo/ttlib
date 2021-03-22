/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       demo_thread.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-22 
 * @brief      demo_thread.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
*  trace 
*/
#define TT_TRACE_MODULE_NAME          "DEMO_PLATFORM_THREAD"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"
#include "../color.h" 

tt_int_t thread1(tt_void_t* priv)
{
    // done
    while (1)
    {
        tt_trace_i("thread, %u, running", tt_thread_self());
        tt_sleep(1);
    }
    
}

tt_int_t thread2(tt_void_t* priv)
{
    // done
    do
    {
        tt_trace_i("thread, %u, running", tt_thread_self());
        tt_sleep(1);
    } while(0);
    
}

tt_void_t tt_demo_platform_thread_main(tt_void_t)
{
	// print title
	tt_print_title("demo platform_thread");

    // platform thread test
	do
    {
        tt_thread_ref_t thread_ref1 = tt_thread_init(tt_null, thread2, tt_null, 0);

        if(thread_ref1)
        {
            tt_int_t retval = 0;
            tt_thread_wait(thread_ref1, -1, &retval);
        }

        // exit
        tt_thread_exit(thread_ref1);

	} while(0);

}