/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       sched_affinity.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-05-06 
 * @brief      sched_affinity.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "pthread_affinity.h"
#include <unistd.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>
#include <sys/types.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_size_t tt_setpthread_affinity(tt_size_t cpu_idx)
{
    // check cpu_idx
    tt_size_t cpu_max = sysconf(_SC_NPROCESSORS_CONF);
    tt_check_return_val(cpu_idx < 0 || cpu_idx > cpu_max, -1);

    // cup set
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_idx, &mask);

    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
        tt_trace_e("pthread affinity set failed");
        return -1;
    }

    return 0;
}
