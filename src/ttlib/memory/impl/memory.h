/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       memory.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      memory.h file
 */

#ifndef TT_MEMORY_IMPL_MEMORY_H
#define TT_MEMORY_IMPL_MEMORY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../allocator.h"


/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/*! init the memory module
 *
 * @param allocator             the allocator
 *
 * @return                      tt_true or tt_false
 */
tt_bool_t                       tt_memory_env_init(tt_allocator_ref_t allocator);

/*! exit the memory module
 *
 */
tt_void_t                       tt_memory_env_exit(tt_void_t);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif