/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       static_fixed_pool.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      static_fixed_pool.h file
 */

#ifndef TT_MEMORY_IMPL_STATIC_FIXED_POOL_H
#define TT_MEMORY_IMPL_STATIC_FIXED_POOL_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the static fixed pool ref type
 *
 * <pre>
 *  ---------------------------------------------------------------------------
 * |  head   |      used       |                    data                       |
 *  ---------------------------------------------------------------------------
 *               |
 *              pred
 * </pre>
 */
typedef __tt_typeref__(static_fixed_pool);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init pool
 *
 * @param data              the data address
 * @param size              the data size
 * @param item_size         the item size
 * @param for_small         add data size field at head for the small allocator 
 *
 * @return                  the pool
 */
tt_static_fixed_pool_ref_t  tt_static_fixed_pool_init(tt_byte_t *data, tt_size_t size, tt_size_t item_size, tt_bool_t for_small);

/*! exit pool
 *
 * @param pool              the pool
 */
tt_void_t                   tt_static_fixed_pool_exit(tt_static_fixed_pool_ref_t pool);

/*! the item count
 *
 * @param pool              the pool
 *
 * @return                  the item count
 */
tt_size_t                   tt_static_fixed_pool_size(tt_static_fixed_pool_ref_t pool);

/*! the item maximum count
 *
 * @param pool              the pool
 *
 * @return                  the item maximum count
 */
tt_size_t                   tt_static_fixed_pool_maxn(tt_static_fixed_pool_ref_t pool);

/*! the pool is full?
 *
 * @param pool              the pool
 *
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_static_fixed_pool_full(tt_static_fixed_pool_ref_t pool);

/*! the pool is null?
 *
 * @param pool              the pool
 *
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_static_fixed_pool_null(tt_static_fixed_pool_ref_t pool);

/*! clear the pool
 *
 * @param pool              the pool
 *
 * @return                  tt_void_t
 */
tt_void_t                   tt_static_fixed_pool_clear(tt_static_fixed_pool_ref_t pool);

/*! malloc data
 *
 * @param pool              the pool
 * 
 * @return                  the data
 */
tt_pointer_t                tt_static_fixed_pool_malloc(tt_static_fixed_pool_ref_t pool __tt_debug_decl__);

/*! free data
 *
 * @param pool              the pool
 * 
 * @param data              the data will free
 * 
 * @return                  tt_true or tt_false
 */
tt_bool_t                   tt_static_fixed_pool_free(tt_static_fixed_pool_ref_t pool, tt_pointer_t data __tt_debug_decl__);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif