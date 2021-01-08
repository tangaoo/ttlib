/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       spinlock.c
 * @ingroup    platform
 * @author     tango
 * @date       2020-11-19 
 * @brief      spinlock.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "spinlock.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private interfaces
 */
tt_spinlock_t* tt_spinlock_init_impl(tt_spinlock_t * spinlock)
{
    ///check
    tt_assert(spinlock);
    return (tt_spinlock_t*)tt_mutex_init_impl((tt_mutex_t *)spinlock);
}

tt_void_t tt_spinlock_exit_impl(tt_spinlock_t * spinlock)
{
        ///check
    tt_assert(spinlock);
    tt_mutex_exit_impl((tt_mutex_t *)spinlock);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_spinlock_ref_t tt_spinlock_init(tt_void_t)
{
    return (tt_spinlock_ref_t)tt_mutex_init();
}

tt_void_t tt_spinlock_exit(tt_spinlock_ref_t spinlock)
{
    ///check
    tt_assert(spinlock);
    tt_mutex_exit((tt_mutex_ref_t)spinlock);
}

tt_bool_t tt_spinlock_entry(tt_spinlock_ref_t spinlock)
{
    ///check
    tt_assert(spinlock);
    return tt_mutex_entry((tt_mutex_ref_t)spinlock);
}

tt_bool_t tt_spinlock_entry_try(tt_spinlock_ref_t spinlock)
{
    ///check
    tt_assert(spinlock);
    return tt_mutex_entry_try((tt_mutex_ref_t)spinlock);
}

tt_bool_t tt_spinlock_leave(tt_spinlock_ref_t spinlock)
{
    ///check
    tt_assert(spinlock);
    return tt_mutex_leave((tt_mutex_ref_t)spinlock);
}