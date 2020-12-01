/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       allocator.h
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      allocator.h file
 */

#ifndef TT_MEMORY_ALLOCATOR_H
#define TT_MEMORY_ALLOCATOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define tt_allocator_malloc(allocator, size)                         tt_allocator_malloc_(allocator, size __tt_debug_val__)
#define tt_allocator_malloc0(allocator, size)                        tt_allocator_malloc0_(allocator, size __tt_debug_val__)
#define tt_allocator_nalloc(allocator, item, size)                   tt_allocator_nalloc_(allocator, item, size __tt_debug_val__)
#define tt_allocator_nalloc0(allocator, item, size)                  tt_allocator_nalloc0_(allocator, item, size __tt_debug_val__)
#define tt_allocator_ralloc(allocator, data, size)                   tt_allocator_ralloc_(allocator, (tt_pointer_t)data, size __tt_debug_val__)
#define tt_allocator_free(allocator, data)                           tt_allocator_free_(allocator, (tt_pointer_t)data __tt_debug_val__)

#define tt_allocator_large_malloc(allocator, size, real)             tt_allocator_large_malloc_(allocator, size, real __tt_debug_val__)
#define tt_allocator_large_malloc0(allocator, size, real)            tt_allocator_large_malloc0_(allocator, size, real __tt_debug_val__)
#define tt_allocator_large_nalloc(allocator, item, size, real)       tt_allocator_large_nalloc_(allocator, item, size, real __tt_debug_val__)
#define tt_allocator_large_nalloc0(allocator, item, size, real)      tt_allocator_large_nalloc0_(allocator, item, size, real __tt_debug_val__)
#define tt_allocator_large_ralloc(allocator, data, size, real)       tt_allocator_large_ralloc_(allocator, (tt_pointer_t)data, size, real __tt_debug_val__)
#define tt_allocator_large_free(allocator, data)                     tt_allocator_large_free_(allocator, (tt_pointer_t)data __tt_debug_val__)

#define tt_allocator_align_malloc(allocator, size, align)            tt_allocator_align_malloc_(allocator, size, align __tt_debug_val__)
#define tt_allocator_align_malloc0(allocator, size, align)           tt_allocator_align_malloc0_(allocator, size, align __tt_debug_val__)
#define tt_allocator_align_nalloc(allocator, item, size, align)      tt_allocator_align_nalloc_(allocator, item, size, align __tt_debug_val__)
#define tt_allocator_align_nalloc0(allocator, item, size, align)     tt_allocator_align_nalloc0_(allocator, item, size, align __tt_debug_val__)
#define tt_allocator_align_ralloc(allocator, data, size, align)      tt_allocator_align_ralloc_(allocator, (tt_pointer_t)data, size, align __tt_debug_val__)
#define tt_allocator_align_free(allocator, data, align)              tt_allocator_align_free_(allocator, (tt_pointer_t)data, align __tt_debug_val__)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

/// the allocator type enum
typedef enum __tt_allocator_type_e
{
    TT_ALLOCATOR_TYPE_NONE      = 0,
    TT_ALLOCATOR_TYPE_DEFAULT   = 1,
    TT_ALLOCATOR_TYPE_NATIVE    = 2,
    TT_ALLOCATOR_TYPE_STATIC    = 3,
    TT_ALLOCATOR_TYPE_LARGE     = 4,
    TT_ALLOCATOR_TYPE_SMALL     = 5

}tt_allocator_type_e;

/// the allocator flag
typedef enum __tt_allocator_flag_e
{
    TT_ALLOCATOR_FLAG_NONE      = 0,
    TT_ALLOCATOR_FLAG_NOLOCK    = 1
}tt_allocator_flag_e;

/// the allocator type
typedef struct __tt_allocator_t
{
    /// the type
    tt_uint32_t                 type : 16;

    /// the flag
    tt_uint32_t                 flag : 16;

    /// the lock
    tt_spinlock_t               lock;

    /*! malloc data
     *
     * @param allocator         the allocator 
     * @param size              the size
     *
     * @return                  the data address
     */
    tt_pointer_t                (*malloc)(struct __tt_allocator_t* allocator, tt_size_t size __tt_debug_decl__);
  
    /*! realloc data
     *
     * @param allocator         the allocator 
     * @param data              data addr will realloc 
     * @param size              the size
     *
     * @return                  the data address
     */
    tt_pointer_t                (*ralloc)(struct __tt_allocator_t* allocator, tt_pointer_t data, tt_size_t size __tt_debug_decl__);    

    /*! free data
     *
     * @param allocator         the allocator 
     * @param data              the data will free
     *
     * @return                  tt_true or tt_false
     */
    tt_bool_t                   (*free)(struct __tt_allocator_t* allocator, tt_pointer_t data __tt_debug_decl__); 

    /*! malloc large data
     *
     * @param allocator         the allocator 
     * @param size              the size
     *
     * @return                  the data address
     */
    tt_pointer_t                (*large_malloc)(struct __tt_allocator_t* allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__);
  
    /*! realloc large data
     *
     * @param allocator         the allocator 
     * @param data              data addr will realloc 
     * @param size              the size
     *
     * @return                  the data address
     */
    tt_pointer_t                (*large_ralloc)(struct __tt_allocator_t* allocator, tt_pointer_t data, tt_size_t size, tt_size_t* real __tt_debug_decl__);    

    /*! free large data
     *
     * @param allocator         the allocator 
     * @param data              the data will free
     *
     * @return                  tt_true or tt_false
     */
    tt_bool_t                   (*large_free)(struct __tt_allocator_t* allocator, tt_pointer_t data __tt_debug_decl__); 

    /*! clear allocator
     *
     * @param allocator         the allocator 
     */
    tt_void_t                   (*clear)(struct __tt_allocator_t* allocator); 

    /*! exit allocator
     *
     * @param allocator         the allocator 
     */
    tt_void_t                   (*exit)(struct __tt_allocator_t* allocator); 

}tt_allocator_t, *tt_allocator_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! the allocator
 *
 * @return                      the allocator
 */
tt_allocator_ref_t              tt_allocator(tt_void_t);

/*! the allocator type
 *
 * @param allocator             the allocator
 *
 * @return                      the allocator type
 */
tt_size_t                       tt_allocator_type(tt_allocator_ref_t allocator);       

/*! malloc it
 *
 * @param allocator             the allocator
 * @param size                  the size
 *
 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_malloc_(tt_allocator_ref_t allocator, tt_size_t size __tt_debug_decl__);

/*! malloc it and set 0
 *
 * @param allocator             the allocator
 * @param size                  the size
 *
 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_malloc0_(tt_allocator_ref_t allocator, tt_size_t size __tt_debug_decl__);

/*! nalloc it
 *
 * @param allocator             the allocator
 * @param item                  the item count
 * @param size                  the item size
 *
 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size __tt_debug_decl__);

/*! nalloc it and set 0
 *
 * @param allocator             the allocator
 * @param item                  the item count
 * @param size                  the item size
 *
 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size __tt_debug_decl__);

/*! ralloc 
 *
 * @param allocator             the allocator
 * @param data                  addr will ralloc
 * @param size                  the item size
 *
 * @return                      addr of data ralloc
 */
tt_pointer_t                    tt_allocator_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size __tt_debug_decl__);

/*! free 
 *
 * @param allocator             the allocator
 * @param data                  addr will free
 *
 * @return                      tt_true of tt_false
 */
tt_bool_t                      tt_allocator_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__);

/*! large malloc it
 *
 * @param allocator             the allocator
 * @param size                  the size
 * @param real                  the real allocated size >= size, optional
 *
 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_large_malloc_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__);

/*! large malloc it and set 0
 *
 * @param allocator             the allocator
 * @param size                  the size
 * @param real                  the real allocated size >= size, optional
 * 
 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_large_malloc0_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__);

/*! large nalloc it
 *
 * @param allocator             the allocator
 * @param item                  the item count
 * @param size                  the item size
 * @param real                  the real allocated size >= size, optional
 * 
 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_large_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t* real __tt_debug_decl__);

/*! large nalloc it and set 0
 *
 * @param allocator             the allocator
 * @param item                  the item count
 * @param size                  the item size
 * @param real                  the real allocated size >= size, optional
 * 
 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_large_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t* real __tt_debug_decl__);

/*! large ralloc 
 *
 * @param allocator             the allocator
 * @param data                  addr will ralloc
 * @param size                  the item size
 * @param real                  the real allocated size >= size, optional
 * 
 * @return                      addr of data ralloc
 */
tt_pointer_t                    tt_allocator_large_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size, tt_size_t* real __tt_debug_decl__);

/*! large free 
 *
 * @param allocator             the allocator
 * @param data                  addr will free
 *
 * @return                      tt_true of tt_false
 */
tt_bool_t                       tt_allocator_large_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__);

/*! align malloc it
 *
 * @param allocator             the allocator
 * @param size                  the size
 * @param align                 the alignment bytes

 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_align_malloc_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t align __tt_debug_decl__);

/*! align malloc it and set 0
 *
 * @param allocator             the allocator
 * @param size                  the size
 * @param align                 the alignment bytes

 * @return                      addr of data malloc
 */
tt_pointer_t                    tt_allocator_align_malloc0_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t align __tt_debug_decl__);

/*! align nalloc it
 *
 * @param allocator             the allocator
 * @param item                  the item count 
 * @param size                  the size
 * @param align                 the alignment bytes

 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_align_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t align __tt_debug_decl__);

/*! align nalloc it and set 0
 *
 * @param allocator             the allocator
 * @param item                  the item count
 * @param size                  the size
 * @param align                 the alignment bytes

 * @return                      addr of data nalloc
 */
tt_pointer_t                    tt_allocator_align_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t align __tt_debug_decl__);

/*! align ralloc it
 *
 * @param allocator             the allocator
 * @param data                  addr of data will ralloc
 * @param size                  the size
 * @param align                 the alignment bytes

 * @return                      addr of data ralloc
 */
tt_pointer_t                    tt_allocator_align_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size, tt_size_t align __tt_debug_decl__);

/*! align free 
 *
 * @param allocator             the allocator
 * @param data                  addr will free
 * @param align                 the alignment bytes
 *
 * @return                      tt_true or tt_false
 */
tt_bool_t                       tt_allocator_align_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__);

/*! clear it
 *
 * @param allocator             the allocator 
 */
tt_void_t                       tt_allocator_clear(tt_allocator_ref_t allocator);

/*! exit it
 *
 * @param allocator             the allocator 
 */
tt_void_t                       tt_allocator_exit(tt_allocator_ref_t allocator);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif