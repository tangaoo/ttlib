
#include "ttlib.h"

tt_void_t tt_lib_init(tt_void_t)
{
	/// trace
	tt_trace_init();

}

tt_void_t tt_lib_exit(tt_void_t)
{
	/// trace
	tt_trace_exit();
}