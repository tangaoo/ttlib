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
tt_void_t demo_utils_mix(tt_void_t)
{
    tt_trace_raw("\n");
    tt_trace_d("----------------------- demo_utils_mix");

    if(tt_little_endian()) 
        tt_trace_d("env is little endian");
    else
        tt_trace_d("env is big endian");
}