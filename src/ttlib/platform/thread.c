/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       thread.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-21 
 * @brief      thread.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "thread.h"
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef struct __tt_thread_t
{
    // pthread
    pthread_t   pthread;

    // is joined?
    tt_bool_t   is_joined;
}tt_thread_t;

// the return val type of thread 
typedef tt_pointer_t    tt_thread_retval_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * static implementation
 */

static tt_thread_retval_t tt_thread_func(tt_pointer_t priv) 
{
    // done
    tt_thread_retval_t retval = (tt_thread_retval_t)0;
    tt_value_ref_t     args = (tt_value_ref_t)priv;

    do
    {
        // check
        tt_assert_and_check_break(args);

        // get the thread function
        tt_thread_func_t func = (tt_thread_func_t)args[0].ptr;
        tt_assert_and_check_break(func);

        // get the thread private data
        tt_cpointer_t priv = (tt_cpointer_t)args[1].ptr;

        // free the args before call func
        if(args) tt_free(args);
        args = tt_null;

        // call the thread function
        retval = (tt_thread_retval_t)(tt_size_t)func(priv);

        // free all thread loacal data on the current thread


    } while (0);
    
    // return thread val
    return retval;

}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_thread_ref_t tt_thread_init(tt_char_t const* name, tt_thread_func_t func, tt_cpointer_t priv, tt_size_t stack) 
{
    // check 
    tt_assert_and_check_return_val(func, tt_null);

    // done
    pthread_attr_t attr;
    tt_bool_t      ok = tt_false;
    tt_value_ref_t args = tt_null;
    tt_thread_t*   thread = tt_null;
    do
    {
        // init thread
        thread = (tt_thread_t *)tt_malloc0(sizeof(tt_thread_t));
        tt_assert_and_check_break(thread);

        // init attr
        if(stack)
        {
            if(pthread_attr_init(&attr)) break;
            pthread_attr_setstacksize(&attr, stack);
        }

        // init arguments
        args = (tt_value_ref_t)tt_malloc0(2 * sizeof(tt_value_t));
        tt_assert_and_check_break(args);

        // save function and private data
        args[0].ptr = (tt_pointer_t)func;
        args[1].ptr = (tt_pointer_t)priv;

        // init pthread
        if (pthread_create(&thread->pthread, stack? &attr : tt_null, tt_thread_func, args)) break;

        // ok
        ok = tt_true;

    } while (0);
    
    // exit attr
    pthread_attr_destroy(&attr);

    // faile
    if(!ok)
    {
        // exit arguments
        if(args) tt_free(args);
        args = tt_null;

        // exit thread
        if(thread) tt_free(thread);
        thread = tt_null;
    }

    // ok
    return ok? ((tt_thread_ref_t)thread) : tt_null;
}

tt_void_t tt_thread_exit(tt_thread_ref_t self)
{
    // check
    tt_thread_t* thread = (tt_thread_t *)self;
    tt_assert_and_check_return(thread);

    // detach thread if not joinable
    if(!thread->is_joined) pthread_detach(thread->pthread);

    //free thread 
    tt_free(thread);
}

tt_long_t tt_thread_wait(tt_thread_ref_t self, tt_long_t timeout, tt_int_t* retval)
{
    // check
    tt_thread_t* thread = (tt_thread_t *)self;
    tt_assert_and_check_return_val(thread, -1);

    // wait thread
    tt_long_t       ok = -1;
    tt_pointer_t    ret = tt_null;
    if ((ok = pthread_join(thread->pthread, &ret)) && ok != ESRCH)
    {
        // trace
        tt_trace_e("thread, %p, wait failed, %ld, errno, %d", thread, ok, errno);

        // join failed, detach it
        pthread_detach(thread->pthread);

        return -1;
    }

    // join ok
    thread->is_joined = tt_true;

    // save the return val
    if(retval) *retval = tt_p2s32(ret);

    // ok
    return 1;
}

tt_void_t tt_thread_return(tt_int_t value)
{
    // exit pthread and return value
    pthread_exit((tt_pointer_t)(tt_long_t)value);
}

tt_bool_t tt_thread_suspend(tt_thread_ref_t self)
{
    tt_trace_noimpl();
    return tt_false;
}

tt_bool_t tt_thread_resume(tt_thread_ref_t self)
{
    tt_trace_noimpl();
    return tt_false;
}

tt_size_t tt_thread_self()
{
    return (tt_size_t)pthread_self();
}