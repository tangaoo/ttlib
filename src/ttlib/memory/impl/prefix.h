/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       prefix.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      prefix.h file
 */

#ifndef TT_MEMORY_IMPL_H
#define TT_MEMORY_IMPL_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "../prefix.h"

// the pool data empty head type
typedef struct __tt_pool_data_empty_head_t
{
// #ifdef __tt_debug__
//     // the debug head
//     tt_pool_data_debug_head_t   debug;
// #endif

    tt_size_t                   dumy; // because of empty struct is error on MSCV

}tt_pool_data_empty_head_t, *tt_pool_data_empty_head_ref_t; 

// the pool data head type
typedef struct __tt_pool_data_head_t
{
// #ifdef __tt_debug__
//     // the debug head
//     tt_pool_data_debug_head_t   debug;
// #endif

    // the size
    tt_size_t                   size;

}tt_pool_data_head_t, *tt_pool_data_head_ref_t;

#endif