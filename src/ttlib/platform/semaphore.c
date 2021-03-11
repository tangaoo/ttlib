/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       semaphore.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-06 
 * @brief      semaphore.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "semaphore.h"
#include <time.h>
#include <errno.h>
#include <semaphore.h>

tt_long_t sem_timedwait_bak(sem_t* h, struct timespec* t)
{
    tt_trace_noimpl();
}
static __attribute__((weakref("sem_timedwait_bak"))) sem_timedwait();

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

tt_semaphore_ref_t tt_semaphore_init(tt_size_t value)
{
    // done
    tt_bool_t ok = tt_false;
    sem_t* semaphore = tt_null;
    do
    {
        // make semaphore
        semaphore = (sem_t*)tt_malloc0(sizeof(sem_t));
        tt_assert_and_check_return_val(semaphore, tt_null);

        // init
        if(sem_init(semaphore, 0, value) < 0) break;

        // ok
        ok = tt_true;
    } while (0);

   if(!ok)
   {
       // exit 
       if(semaphore) tt_free(semaphore);
       semaphore = tt_null;
   } 

    // ok
    return (tt_semaphore_ref_t)semaphore;
}

tt_void_t tt_semaphore_exit(tt_semaphore_ref_t semaphore)
{
    // check
    sem_t* h = (sem_t*)semaphore;
    tt_assert_and_check_return(h);

    // exit
    sem_destroy(h);

    // free
    tt_free(h);
}

tt_bool_t tt_semaphore_post(tt_semaphore_ref_t semaphore, tt_size_t post)
{
    // check
    sem_t* h = (sem_t*)semaphore;
    tt_assert_and_check_return_val(h, tt_false);

    // done 
    while(post--)
    {
        if(sem_post(h) < 0) return tt_false;
    }

    // ok
    return tt_true;
}


tt_size_t tt_semaphore_value(tt_semaphore_ref_t semaphore)
{ 
    // check
    sem_t* h = (sem_t*)semaphore;
    tt_assert_and_check_return_val(h, -1);

    // done
    tt_int_t val = 0;
    return !sem_getvalue(h, &val)? (tt_long_t)val : 1;
}

tt_long_t tt_semaphore_wait(tt_semaphore_ref_t semaphore, tt_long_t timeout)
{
    // check
    sem_t* h = (sem_t*)semaphore;
    tt_assert_and_check_return_val(h, -1);

    // init time
    struct timespec t = {0};
    t.tv_sec = time(tt_null);
    if(timeout > 0)
    {
        t.tv_sec  += timeout / 1000;
        t.tv_nsec += timeout % 1000 * 1000000;
    }
    else if(timeout < 0) t.tv_sec += 12 * 30 * 24 * 3600; // one year mean infinity

    // wait
    tt_long_t ok = 0;
    ok = sem_timedwait(h, &t);

    // ok?
    tt_assert_and_check_return_val(ok, 1);

    // timeout
    if(errno == EINTR || errno == EAGAIN || errno == ETIMEDOUT) return 0;

    // failed
    return -1;
}

