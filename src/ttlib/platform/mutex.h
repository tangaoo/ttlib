
/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       mutex.h
 * @ingroup    mutex
 * @author     tango
 * @date       2020-10 
 * @brief      mutex.h file
 */
#ifndef TT_PLATFORM_MUTEX_H
#define TT_PLATFORM_MUTEX_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

#define POSIX   1

#ifdef  POSIX
#include "pthread.h"

typedef pthread_mutex_t    tt_mutex_t;
typedef pthread_mutex_t*   tt_mutex_ref_t;

#endif

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init mutex(not malloc in function)
 *
 * @param mutex             the mutex
 *
 * @return                  the mutex
 */
tt_mutex_t                  tt_mutex_init_impl(tt_mutex_t * mutex);

/*! exit mutex
 *
 * @param mutex             the mutex
 *
 * @return                  void
 */
tt_void_t                   tt_mutex_exit_impl(tt_mutex_t * mutex);

/*! int mutex(dynamic malloc)
 *
 * @return                  the mutex
 */
tt_mutex_ref_t              tt_mutex_init(tt_void_t);

/*! exit mutex
 *
 * @param mutex             the mutex
 *
 * @return                  void
 */
tt_void_t                   tt_mutex_exit(tt_mutex_ref_t mutex);

/*! entry mutex
 *
 * @param mutex             the mutex
 *
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_mutex_entry(tt_mutex_ref_t mutex);

/*! try entry mutex
 *
 * @param mutex             the mutex
 *
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_mutex_entry_try(tt_mutex_ref_t mutex);

/*! leave mutex
 *
 * @param mutex             the mutex
 *
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_mutex_leave(tt_mutex_ref_t mutex);


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif