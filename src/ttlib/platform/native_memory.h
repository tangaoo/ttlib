/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       native_memory.h
 * @ingroup    platform
 * @author     tango
 * @date       2020-11-19 
 * @brief      native_memory.h file
 */

#ifndef TT_PLATFORM_NATIVE_MEMORY_H
#define TT_PLATFORM_NATIVE_MEMORY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/*! native mem init
 *
 * @return              tt_true or tt_false
 */
tt_bool_t               tt_native_memory_init(tt_void_t);

/*! native mem exit
 *
 * @return              tt_void_t
 */
tt_void_t               tt_native_memory_exit(tt_void_t);

/*! native mem malloc
 *
 * @param size          the size
 *
 * @return              the addr of memory malloced
 */
tt_pointer_t            tt_native_memory_malloc(tt_size_t size);

/*! native mem malloc and filled zero
 *
 * @param size          the size
 *
 * @return              the addr of memory malloced
 */
tt_pointer_t            tt_native_memory_malloc0(tt_size_t size);

/*! malloc the native memory with the item count
 *
 * @param item          the item
 * @param count         the count
 *
 * @return              the addr of memory malloced
 */
tt_pointer_t            tt_native_memory_nalloc(tt_size_t item, tt_size_t count);

/*! malloc the native memory with the item count and filled zero
 *
 * @param item          the item
 * @param count         the count
 *
 * @return              the addr of memory malloced
 */
tt_pointer_t            tt_native_memory_nalloc0(tt_size_t item, tt_size_t count);

/*! realloc the native memory
 *
 * @param data          the data addr
 * @param size          the size
 *
 * @return              the addr of memory malloced
 */
tt_pointer_t            tt_native_memory_realloc(tt_pointer_t data, tt_size_t size);

/*! native mem free
 *
 * @param data          the data addr
 *
 * @return              the addr of memory malloced
 */
tt_bool_t               tt_native_memory_free(tt_pointer_t data);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif
