/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       static_large_allocator.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-12-25 
 * @brief      static_large_allocator.h file
 */

#ifndef TT_MEMORY_IMPL_STATIC_LARGE_ALLOCATOR_H
#define TT_MEMORY_IMPL_STATIC_LARGE_ALLOCATOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../allocator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the static large allocator init
 *
 * @param data       the static buffer pointer
 * @param size       the static buffer size
 * @param page_size  the static buffer pointer
 *
 * @return           the allocator
 */
tt_allocator_ref_t   tt_static_large_allocator_init(tt_byte_t* data, tt_size_t size, tt_size_t page_size);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif