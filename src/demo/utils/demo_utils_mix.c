/* //////////////////////////////////////////////////////////////////////////////////////
*  trace 
*/
#define TT_TRACE_MODULE_NAME          "DEMO_UTILS_MIX"
#define TT_TRACE_MODULE_DEBUG         (1)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h" 

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_void_t tt_demo_utils_mix_main(tt_void_t)
{
    tt_trace_raw("\n");
    tt_trace_d("----------------------- demo_utils_mix");

    if(tt_little_endian()) 
        tt_trace_d("env is little endian");
    else
        tt_trace_d("env is big endian");
    
    // sizeof tt_size_t
    tt_trace_i("%d", sizeof(tt_size_t));
}