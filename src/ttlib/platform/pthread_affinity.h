/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       sched_affinity.h
 * @ingroup    platform
 * @author     tango
 * @date       2021-05-06 
 * @brief      sched_affinity.h file
 */

#ifndef TT_SCHED_AFFINITY_H
#define TT_SCHED_AFFINITY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_size_t tt_setpthread_affinity(tt_size_t cpu_idx);

__tt_extern_c_leave__

#endif