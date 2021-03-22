/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       native_allocator.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      native_allocator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME          "NATIVE_ALLOCATOR"
#define TT_TRACE_MODULE_DEBUG         (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "native_allocator.h"
#include "../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private implementation
 */
static tt_pointer_t tt_native_allocator_malloc_(tt_allocator_ref_t allocator, tt_size_t size __tt_debug_decl__)
{
    /// trace
    tt_trace_d("malloc(%lu), at %s(), %lu, %s", size __tt_debug_args__);

    /// malloc it
    return tt_native_memory_malloc(size);
}

static tt_pointer_t tt_native_allocator_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size __tt_debug_decl__)
{
    /// trace
    tt_trace_d("realloc(%lu) at %s(): %lu, %s", size __tt_debug_args__);

    /// malloc it
    return tt_native_memory_realloc(data, size);
}

static tt_bool_t tt_native_allocator_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__)
{
    /// trace
    tt_trace_d("free(%lu) at %s(): %lu, %s", data __tt_debug_args__);

    /// malloc it
    return tt_native_memory_free(data);
}

static tt_bool_t tt_native_allocator_instance_init(tt_handle_t handle, tt_pointer_t priv)
{
    /// check
    tt_allocator_ref_t allocator = (tt_allocator_ref_t)handle;
    tt_check_return_val(allocator, tt_false);

    /* init the native memory first
     *
     * because this allocator may be called before tb_init()
     */
    if(!tt_native_memory_init()) return tt_false; /// TODO
    
    /// init native allocator
    allocator->type           = TT_ALLOCATOR_TYPE_NATIVE;
    allocator->flag           = TT_ALLOCATOR_FLAG_NOLOCK;
    allocator->malloc         = tt_native_allocator_malloc_;
    allocator->ralloc         = tt_native_allocator_ralloc_;
    allocator->free           = tt_native_allocator_free_;
    
    return tt_true;
}

tt_allocator_ref_t tt_native_allocator(tt_void_t)
{
    static tt_allocator_t s_allocator = {0};

    tt_native_allocator_instance_init(&s_allocator, tt_null);

    return &s_allocator;
}


