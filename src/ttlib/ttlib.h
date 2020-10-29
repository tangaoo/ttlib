
#ifndef _TT_LIB_
#define _TT_LIB_

#include "prefix/prefix.h"
#include "buffer/buffer.h"
#include "container/container.h"
#include "container/iterator.h"
#include "algorithm/algorithm.h"
#include "utils/utils.h"

__tt_extern_c_enter__

tt_void_t tt_lib_init(tt_void_t);
tt_void_t tt_lib_exit(tt_void_t);

__tt_extern_c_leave__

#endif