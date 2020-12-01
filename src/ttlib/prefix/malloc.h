/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       malloc.h
 * @ingroup    prefix
 * @author     tango
 * @date       2020-11-19 
 * @brief      malloc.h file
 */
#ifndef TT_PREFIX_MALLOC_H
#define TT_PREFIX_MALLOC_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "keyword.h"
#include "type.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define tt_malloc(size)                         tt_allocator_malloc_(tt_allocator(), size __tt_debug_val__)
#define tt_malloc0(size)                        tt_allocator_malloc0_(tt_allocator(), size __tt_debug_val__)
#define tt_nalloc(item, size)                   tt_allocator_nalloc_(tt_allocator(), item, size __tt_debug_val__)
#define tt_nalloc0(item, size)                  tt_allocator_nalloc0_(tt_allocator(), item, size __tt_debug_val__)
#define tt_ralloc(data, size)                   tt_allocator_ralloc_(tt_allocator(), item, size __tt_debug_val__)
#define tt_free(data)                           tt_allocator_free_(tt_allocator(), data __tt_debug_val__)

#define tt_large_malloc(size, real)             tt_allocator_large_malloc_(tt_allocator(), size, real __tt_debug_val__)
#define tt_large_malloc0(size, real)            tt_allocator_large_malloc0_(tt_allocator(), size, real __tt_debug_val__)
#define tt_large_nalloc(item, size, real)       tt_allocator_large_nalloc_(tt_allocator(), item, size, real __tt_debug_val__)
#define tt_large_nalloc0(item, size, real)      tt_allocator_large_nalloc0_(tt_allocator(), item, size, real __tt_debug_val__)
#define tt_large_ralloc(data, size, real)       tt_allocator_large_ralloc_(tt_allocator(), item, size, real __tt_debug_val__)
#define tt_large_free(data)                     tt_allocator_large_free_(tt_allocator(), data __tt_debug_val__)

#define tt_align_malloc(size, align)            tt_allocator_align_malloc_(tt_allocator(), size, align __tt_debug_val__)
#define tt_align_malloc0(size, align)           tt_allocator_align_malloc0_(tt_allocator(), size, align __tt_debug_val__)
#define tt_align_nalloc(item, size, align)      tt_allocator_align_nalloc_(tt_allocator(), item, size, align __tt_debug_val__)
#define tt_align_nalloc0(item, size, align)     tt_allocator_align_nalloc0_(tt_allocator(), item, size, align __tt_debug_val__)
#define tt_align_ralloc(data, size, align)      tt_allocator_align_ralloc_(tt_allocator(), item, size, align __tt_debug_val__)
#define tt_align_free(data, align)              tt_allocator_align_free_(tt_allocator(), data, align __tt_debug_val__)

/* //////////////////////////////////////////////////////////////////////////////////////
 * declaration
 */
struct __tt_allocator_t;
struct __tt_allocator_t*  tt_allocator(tt_void_t);
tt_pointer_t              tt_allocator_malloc_(struct __tt_allocator_t* allocator, tt_size_t size __tt_debug_decl__);
tt_pointer_t              tt_allocator_malloc0_(struct __tt_allocator_t* allocator, tt_size_t size __tt_debug_decl__);
tt_pointer_t              tt_allocator_nalloc_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size __tt_debug_decl__);
tt_pointer_t              tt_allocator_nalloc0_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size __tt_debug_decl__);
tt_pointer_t              tt_allocator_ralloc_(struct __tt_allocator_t* allocator, tt_pointer_t data, tt_size_t size __tt_debug_decl__);
tt_bool_t                 tt_allocator_free_(struct __tt_allocator_t* allocator, tt_pointer_t data __tt_debug_decl__);

tt_pointer_t              tt_allocator_large_malloc_(struct __tt_allocator_t* allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__);
tt_pointer_t              tt_allocator_large_malloc0_(struct __tt_allocator_t* allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__);
tt_pointer_t              tt_allocator_large_nalloc_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size, tt_size_t* real __tt_debug_decl__);
tt_pointer_t              tt_allocator_large_nalloc0_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size, tt_size_t* real __tt_debug_decl__);
tt_pointer_t              tt_allocator_large_ralloc_(struct __tt_allocator_t* allocator, tt_pointer_t data, tt_size_t size, tt_size_t* real __tt_debug_decl__);
tt_bool_t                 tt_allocator_large_free_(struct __tt_allocator_t* allocator, tt_pointer_t data __tt_debug_decl__);

tt_pointer_t              tt_allocator_align_malloc_(struct __tt_allocator_t* allocator, tt_size_t size, tt_size_t align __tt_debug_decl__);
tt_pointer_t              tt_allocator_align_malloc0_(struct __tt_allocator_t* allocator, tt_size_t size, tt_size_t align __tt_debug_decl__);
tt_pointer_t              tt_allocator_align_nalloc_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size, tt_size_t align __tt_debug_decl__);
tt_pointer_t              tt_allocator_align_nalloc0_(struct __tt_allocator_t* allocator, tt_size_t item,tt_size_t size, tt_size_t align __tt_debug_decl__);
tt_pointer_t              tt_allocator_align_ralloc_(struct __tt_allocator_t* allocator, tt_pointer_t data, tt_size_t size, tt_size_t align __tt_debug_decl__);
tt_bool_t                 tt_allocator_align_free_(struct __tt_allocator_t* allocator, tt_pointer_t data __tt_debug_decl__);

#endif