/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       native_allocator.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      native_allocator.h file
 */

#ifndef TT_MEMORY_NATIVE_ALLOCATOR_H
#define TT_MEMORY_NATIVE_ALLOCATOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "allocator.h"

/*! the global native allocator
 *
 * @return              the global native allocator
 */
tt_allocator_ref_t      tt_native_allocator(tt_void_t);

#endif
