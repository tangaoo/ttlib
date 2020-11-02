/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       mutex.c
 * @ingroup    platform
 * @author     tango
 * @date       2020-10 
 * @brief      mutex.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mutex.h"


#ifdef  POSIX
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
    return tt_null;
}

tt_void_t tt_mutex_exit(tt_mutex_ref_t mutex) // TODO
{}

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

#else
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