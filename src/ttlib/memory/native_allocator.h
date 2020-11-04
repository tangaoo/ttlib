/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       native_allocator.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11 
 * @brief      native_allocator.h file
 */
#ifndef  TT_MEMORY_NATIVE_ALLOCATOR_H
#define  TT_MEMORY_NATIVE_ALLOCATOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "allocator.h"

/*! the global native allocator
 *
 * @return              the global native allocator
 */
tt_allocator_ref_t      tt_native_allocator(tt_void_t);

#endif
