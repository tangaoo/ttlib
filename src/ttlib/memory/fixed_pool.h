/*!The TT Library
 *
 * @Copyright (C) 2019-2020, TTLIB
 *
 * @file       fixed_pool.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-24 
 * @brief      fixed_pool.h file
 */

#ifndef TT_MEMORY_FIXED_POOL_H
#define TT_MEMORY_FIXED_POOL_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "allocator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define tt_fixed_pool_malloc(pool)          tt_fixed_pool_malloc_(pool __tt_debug_val__)
#define tt_fixed_pool_malloc0(pool)         tt_fixed_pool_malloc0_(pool __tt_debug_val__)
#define tt_fixed_pool_free(pool, item)      tt_fixed_pool_free(pool, (tt_pointer_t)item __tt_debug_val__)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/*! the fixed pool ref type
 * 
 * <pre>
 *
 * current:
 *         -----------
 *        |           |
 *  --------------    |
 * |     slot     |<--
 * |--------------|
 * ||||||||||||||||  
 * |--------------| 
 * |              | 
 * |--------------| 
 * |              | 
 * |--------------| 
 * ||||||||||||||||  
 * |--------------| 
 * |||||||||||||||| 
 * |--------------| 
 * |              | 
 *  --------------  
 *
 * partial:
 *
 *  --------------       --------------               --------------
 * |     slot     | <=> |     slot     | <=> ... <=> |     slot     |
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     |              |             |              |
 * |--------------|     |--------------|             |--------------|
 * |              |     ||||||||||||||||             |              |
 * |--------------|     |--------------|             |--------------|
 * |              |     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             |              |
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     |              |             |              |
 * |--------------|     |--------------|             |--------------|
 * |              |     |              |             ||||||||||||||||
 *  --------------       --------------               --------------
 *
 * full:
 *
 *  --------------       --------------               --------------
 * |     slot     | <=> |     slot     | <=> ... <=> |     slot     |
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 * |--------------|     |--------------|             |--------------|
 * ||||||||||||||||     ||||||||||||||||             ||||||||||||||||
 *  --------------       --------------               --------------
 *
 * slot:
 *
 *  -------------- ------------------------>|
 * |     head     |                         |
 * |--------------|                         |
 * |||   item     |                         |  
 * |--------------|                         |
 * |||   item     |                         |  
 * |--------------|                         | data
 * |||   item     |                         |  
 * |--------------|                         | 
 * |      ...     |                         |  
 * |--------------|                         | 
 * |||   item     |                         | 
 *  -------------- ------------------------>|
 *
 * slot list:
 * 
 *  -------------------------------------------------------
 *  | slot_1  | slot_2 |   ......            |  slot_n    |
 *  -------------------------------------------------------
 * </pre>
 */
typedef __tt_typeref__(fixed_pool);

/// the item init func type
typedef tt_bool_t             (*tt_fixed_pool_item_init_func_t)(tt_pointer_t data, tt_cpointer_t priv);

/// the item exit func type
typedef tt_void_t             (*tt_fixed_pool_item_exit_func_t)(tt_pointer_t data, tt_cpointer_t priv);

/// the item walk func type
typedef tt_bool_t             (*tt_fixed_pool_item_walk_func_t)(tt_pointer_t data, tt_cpointer_t priv);

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init the fixed_pool
 *
 * @param large_allocator     the large allocator, use the global allocator if be null
 * @param slot_size           the item count per-slot, use the default size if be null
 * @param item_size           the item size
 * @param item_init           the item init func
 * @param item_exit           the item exit func
 * @param priv                the private data 
 *
 * @return                    the fixed pool
 */
tt_fixed_pool_ref_t           tt_fixed_pool_init(tt_allocator_ref_t large_allocator, tt_size_t slot_size, tt_size_t item_size, tt_fixed_pool_item_init_func_t item_init, tt_fixed_pool_item_exit_func_t item_exit, tt_cpointer_t priv);


/*! exit the fixed pool
 *
 * @param pool                the pool
 *
 * @return                    tt_void
 */
tt_void_t                     tt_fixed_pool_exit(tt_fixed_pool_ref_t pool);

/*! the used item count
 *
 * @param pool                the pool
 *
 * @return                    the size of fixed pool
 */
tt_size_t                     tt_fixed_pool_size(tt_fixed_pool_ref_t pool);

/*! the item size
 *
 * @param pool                the pool
 *
 * @return                    the size of the item
 */
tt_size_t                     tt_fixed_pool_item_size(tt_fixed_pool_ref_t pool);

/*! clear the pool
 *
 * @param pool                the pool
 *
 * @return                    tt_void
 */
tt_void_t                     tt_fixed_pool_clear(tt_fixed_pool_ref_t pool);

/*! malloc data
 *
 * @param pool                the pool
 *
 * @return                    the data
 */
tt_pointer_t                  tt_fixed_pool_malloc_(tt_fixed_pool_ref_t pool __tt_debug_decl__);

/*! malloc data and clear it
 *
 * @param pool                the pool
 *
 * @return                    the data
 */
tt_pointer_t                  tt_fixed_pool_malloc0_(tt_fixed_pool_ref_t pool __tt_debug_decl__);

/*! free data
 *
 * @param pool                the pool
 * @param data                the data
 *
 * @return                    tt_true or tt_false
 */
tt_bool_t                     tt_fixed_pool_free_(tt_fixed_pool_ref_t pool, tt_pointer_t data __tt_debug_decl__);




/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif