/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       time.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-03 
 * @brief      time.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "time.h"
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_void_t tt_usleep(tt_size_t us)
{
    usleep(us);
}

tt_void_t tt_msleep(tt_size_t ms)
{
    tt_usleep(ms * 1000);
}

tt_void_t tt_sleep(tt_size_t s)
{
    tt_msleep(s * 1000);
}

tt_hong_t tt_uclock(tt_void_t)
{
    tt_timeval_t tv = {0};
    if(!tt_gettimeofday(&tv, tt_null)) return -1;
    return (tt_hong_t)tv.tv_sec * 1000000 + tv.tv_usec;
}

tt_hong_t tt_mclock(tt_void_t)
{
    tt_timeval_t tv = {0};
    if(!tt_gettimeofday(&tv, tt_null)) return -1;
    return (tt_hong_t)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

tt_bool_t tt_gettimeofday(tt_timeval_t* tv, tt_timezone_t* tz)
{
    // gettimeofday
    struct timeval ttv  = {0};
    struct timezone ttz = {0};
    if(gettimeofday(&ttv, &ttz)) return tt_false;

    // tv
    if(tv)
    {
        tv->tv_sec = (tt_time_t)ttv.tv_sec;
        tv->tv_usec = (tt_time_t)ttv.tv_usec;
    }

    // tz
    if(tz)
    {
        tz->tz_minuteswest = ttz.tz_minuteswest;
        tz->tz_dsttime = ttz.tz_dsttime;  
    }

    // ok
    return tt_true;
}
