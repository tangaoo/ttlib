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
#include "sched_affinity.h"

int attach_cpu(int cpu_index)
{
    int cpu_num = sysconf(_SC_NPROCESSORS_CONF);
    if (cpu_index < 0 || cpu_index >= cpu_num)
    {
        printf("cpu index ERROR!\n");
        return -1;
    }

    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu_index, &mask);

    if (pthread_setaffinity_np(pthread_self(), sizeof(mask), &mask) < 0)
    {
        printf("set affinity np ERROR!\n");
        return -1;
    }

    return 0;
}
