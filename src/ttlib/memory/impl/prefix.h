/*
 * @Copyright (C)  2020  .Tango. all right reserved
 * @file       prefix.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11 
 * @brief      prefix.h file
 */

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