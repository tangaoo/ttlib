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

/* //////////////////////////////////////////////////////////////////////////////////////
 * private interfaces
 */
tt_mutex_t tt_mutex_init_impl(tt_mutex_t * mutex)
{
    /// @note we cannot use asset/trace because them will use mutex
    return (mutex && !pthread_mutex_init(mutex, tt_null) ? (tt_mutex_ref_t)mutex : tt_null);
}

tt_void_t tt_mutex_exit_impl(tt_mutex_t * mutex)
{
    /// exit it
    if(mutex) pthread_mutex_destroy(mutex);
}