/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       spinlock2.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-27 
 * @brief      spinlock2.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "spinlock2.h"
#include "atomic.h"

// the initial value
#define TT_SPINLOCK_INIT            TT_ATOMIC_FLAG_INIT

static __tt_inline_force__ tt_bool_t tt_spinlock_init(tt_spinlock_ref_t lock)
{

}
