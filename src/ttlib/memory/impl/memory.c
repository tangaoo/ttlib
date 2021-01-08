/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       memory.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      memory.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "memory.h"
#include "../native_allocator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * globals
 */
__tt_extern_c__ extern tt_allocator_ref_t g_allocator;

tt_bool_t tt_memory_env_init(tt_allocator_ref_t allocator)
{
    tt_bool_t ret = tt_false;

    do
    {
        /// init native allocator
        if(!tt_native_memory_init()) break;
        /// done
        g_allocator = allocator? allocator : (tt_allocator_ref_t)(tt_native_allocator());        
        tt_assert(g_allocator);

        ret = tt_true;
    }while(0);

    if(!ret) tt_memory_env_exit();

    return ret;
}

tt_void_t tt_memory_env_exit(tt_void_t)
{
    tt_native_memory_exit();
}
