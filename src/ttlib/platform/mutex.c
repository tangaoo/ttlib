/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       mutex.c
 * @ingroup    platform
 * @author     tango
 * @date       2020-11-19 
 * @brief      mutex.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mutex.h"


#if defined(TT_POSIX) && TT_POSIX
/* //////////////////////////////////////////////////////////////////////////////////////
 * private interfaces
 */
tt_mutex_t* tt_mutex_init_impl(tt_mutex_t * mutex)
{
    /// @note we cannot use asset/trace because them will use mutex
    return (mutex && !pthread_mutex_init(mutex, tt_null) ? (tt_mutex_ref_t)mutex : tt_null);
}

tt_void_t tt_mutex_exit_impl(tt_mutex_t * mutex)
{
    /// exit it
    if(mutex) pthread_mutex_destroy(mutex);
}

tt_bool_t tt_mutex_enter_without_profiler(tt_mutex_ref_t mutex)
{
    // check, @note we cannot use asset/trace because them will use mutex
    tt_check_return_val(mutex, tt_false);
    
    return (pthread_mutex_lock((pthread_mutex_t*)mutex) == 0 ? tt_true : tt_false);
}

tt_bool_t tt_mutex_enter_try_without_profiler(tt_mutex_ref_t mutex)
{
    // check, @note we cannot use asset/trace because them will use mutex
    tt_check_return_val(mutex, tt_false);
    
    return pthread_mutex_trylock((pthread_mutex_t*)mutex) == 0 ? tt_true : tt_false;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_mutex_ref_t tt_mutex_init(tt_void_t) // TODO
{
    /// make mutex
    tt_mutex_t *mutex = (tt_mutex_ref_t)tt_malloc0(sizeof(tt_mutex_t));
    tt_assert(mutex);
    tt_check_return_val(mutex, tt_null);

    tt_mutex_ref_t pmutex = tt_mutex_init_impl(mutex);
    if(pmutex) return pmutex;
    else
    {
        if(mutex) tt_free((tt_pointer_t)mutex);
        return tt_null;
    }   
}

tt_void_t tt_mutex_exit(tt_mutex_ref_t mutex) // TODO
{
    tt_assert(mutex);
    tt_check_return(mutex);

    if(mutex)
    {
        tt_mutex_exit_impl(mutex);
        tt_free((tt_pointer_t)mutex);
    }
}

tt_bool_t tt_mutex_entry(tt_mutex_ref_t mutex)
{
    return tt_mutex_enter_without_profiler(mutex);
}

tt_bool_t tt_mutex_entry_try(tt_mutex_ref_t mutex)
{
    return tt_mutex_enter_try_without_profiler(mutex);
}

tt_bool_t tt_mutex_leave(tt_mutex_ref_t mutex)
{
    return pthread_mutex_unlock((pthread_mutex_t*)mutex) == 0 ? tt_true : tt_false;
}

#elif defined(TT_POSIX_NO) && TT_POSIX_NO
/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_mutex_t* tt_mutex_init_impl(tt_mutex_t * mutex)
{
    /// @note we cannot use asset/trace because them will use mutex
    return mutex;
}

tt_void_t tt_mutex_exit_impl(tt_mutex_t * mutex)
{
    /// exit it
    
}

tt_mutex_ref_t tt_mutex_init(tt_void_t) // TODO
{
    return tt_null;
}

tt_void_t tt_mutex_exit(tt_mutex_ref_t mutex) // TODO
{}

tt_bool_t tt_mutex_entry(tt_mutex_ref_t mutex)
{
    return tt_true;
}

tt_bool_t tt_mutex_entry_try(tt_mutex_ref_t mutex)
{
    return tt_true;
}

tt_bool_t tt_mutex_leave(tt_mutex_ref_t mutex)
{
    return tt_true;
}

#endif