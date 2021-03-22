/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       allocator.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11-19 
 * @brief      allocator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME          "ALLOCATOR"
#define TT_TRACE_MODULE_DEBUG         (0)

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "allocator.h"

/// the allocator
tt_allocator_ref_t g_allocator;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_allocator_ref_t tt_allocator(tt_void_t)
{
    return g_allocator;
}

tt_size_t tt_allocator_type(tt_allocator_ref_t allocator)
{
    /// check
    tt_assert(allocator);
    
    return (tt_size_t)allocator->type;
}

tt_pointer_t tt_allocator_malloc_(tt_allocator_ref_t allocator, tt_size_t size __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock)
        tt_spinlock_entry(&allocator->lock);
    
    /// malloc it
    tt_pointer_t data = tt_null;
    if(allocator->malloc)
        data = allocator->malloc(allocator, size __tt_debug_args__);
    else if(allocator->large_malloc)
        data = allocator->large_malloc(allocator, size, tt_null __tt_debug_args__);

    /// trace
    tt_trace_d("malloc(%lu), %p, at %s(), %d, %s", size, data __tt_debug_args__);
    /// check
    tt_assertf(data, "malloc(%lu) failed!", size);
    tt_assertf(!(((tt_size_t)data) & (TT_POOL_DATA_ALIGN-1)), "malloc(%lu) unalign data, %p", size, data);

    /// leave
    if(!lock)
        tt_spinlock_leave(&allocator->lock);

    return data;
}

tt_pointer_t tt_allocator_malloc0_(tt_allocator_ref_t allocator, tt_size_t size __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    tt_pointer_t data = tt_null;
    /// malloc it
    data = tt_allocator_malloc_(allocator, size __tt_debug_args__);
    if(data) tt_memset(data, 0, size);
    
    return data;
}

tt_pointer_t tt_allocator_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    return tt_allocator_malloc_(allocator, item*size __tt_debug_args__);
}

tt_pointer_t tt_allocator_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    tt_pointer_t data = tt_allocator_malloc_(allocator, item*size __tt_debug_args__);
    if(data) tt_memset(data, 0, item*size);

    return data;
}

tt_pointer_t tt_allocator_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock)
        tt_spinlock_entry(&allocator->lock);
    
    /// malloc it
    tt_pointer_t data_new = tt_null;
    if(allocator->ralloc)
        data_new = allocator->ralloc(allocator, data, size __tt_debug_args__);
    else if(allocator->large_ralloc)
        data_new = allocator->large_ralloc(allocator, data, size, tt_null __tt_debug_args__);

    /// trace
    tt_trace_d("ralloc(%lu): %p at %s(): %d, %s", size, data_new __tt_debug_args__);
    /// check
    tt_assertf(data_new, "ralloc(%lu) failed!", size);
    tt_assertf(!(((tt_size_t)data_new) & (TT_POOL_DATA_ALIGN-1)), "malloc(%lu) unalign data, %p", size, data_new);

    /// leave
    if(!lock)
        tt_spinlock_leave(&allocator->lock);

    return data_new;
}

tt_bool_t tt_allocator_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__)
{
    /// check
    tt_assert(allocator && data);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock)
        tt_spinlock_entry(&allocator->lock);

    /// free
    tt_bool_t ret = tt_false;
    if(allocator->ralloc)
        ret = allocator->free(allocator, data __tt_debug_args__);
    else if(allocator->large_ralloc)
        ret = allocator->large_free(allocator, data __tt_debug_args__);

    /// leave
    if(!lock)
        tt_spinlock_leave(&allocator->lock);

    return ret;
}

tt_pointer_t tt_allocator_large_malloc_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock)
        tt_spinlock_entry(&allocator->lock);
    
    /// malloc it
    tt_pointer_t data = tt_null;
    if(allocator->large_malloc)
        data = allocator->large_malloc(allocator, size, real __tt_debug_args__);
    else if(allocator->malloc)
    {
        if(real) *real = size;
        data = allocator->malloc(allocator, size __tt_debug_args__);        
    }

    /// trace
    tt_trace_d("large_malloc(%lu): %p at %s(): %d, %s", size, data __tt_debug_args__);
    /// check
    tt_assertf(data, "large_malloc(%lu) failed!", size);
    tt_assertf(!(((tt_size_t)data) & (TT_POOL_DATA_ALIGN-1)), "malloc(%lu) unalign data, %p", size, data);
    tt_assert(!real || *real >= size); //large malloc memory large than size

    /// leave
    if(!lock)
        tt_spinlock_leave(&allocator->lock);

    return data;    
}

tt_pointer_t tt_allocator_large_malloc0_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);

    tt_pointer_t data = tt_null;
    data = tt_allocator_large_malloc_(allocator, size, real __tt_debug_args__);
    if(data) tt_memset(data, 0, real ? *real : size);

    return data;
}

tt_pointer_t tt_allocator_large_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);

    return tt_allocator_large_malloc_(allocator, item*size, real __tt_debug_args__);
}

tt_pointer_t tt_allocator_large_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);

    return tt_allocator_large_malloc0_(allocator, item*size, real __tt_debug_args__);
}

tt_pointer_t tt_allocator_large_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size, tt_size_t* real __tt_debug_decl__)
{
    /// check
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock)
        tt_spinlock_entry(&allocator->lock);
    
    /// malloc it
    tt_pointer_t data_new = tt_null;
    if(allocator->large_malloc)
        data_new = allocator->large_ralloc(allocator, data, size, real __tt_debug_args__);
    else if(allocator->malloc)
    {
        *real = size;
        data_new = allocator->ralloc(allocator, data, size __tt_debug_args__);        
    }

    /// trace
    tt_trace_d("large_malloc(%lu): %p at %s(): %d, %s", size, data_new __tt_debug_args__);
    /// check
    tt_assertf(data_new, "large_malloc(%lu) failed!", size);
    tt_assertf(!(((tt_size_t)data_new) & (TT_POOL_DATA_ALIGN-1)), "malloc(%lu) unalign data, %p", size, data);
    tt_assert(!real || *real >= size); //large malloc memory large than size

    /// leave
    if(!lock)
        tt_spinlock_leave(&allocator->lock);

    return data_new;  
}

tt_bool_t tt_allocator_large_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = tt_false;
    lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock) tt_spinlock_entry(&allocator->lock);

    /// free
    tt_bool_t ret = tt_false;
    if(allocator->large_free) ret = allocator->large_free(allocator, data __tt_debug_args__);
    else if(allocator->free) ret = allocator->free(allocator, data __tt_debug_args__);

    /// leave
    if(!lock) tt_spinlock_leave(&allocator->lock);

    return ret;
}

tt_pointer_t tt_allocator_align_malloc_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t align __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);
    tt_assertf(!(align & 3), "invalid align size(%lu)", align);

    /// malloc
    tt_byte_t* data = (tt_byte_t*)tt_allocator_malloc_(allocator, size + align __tt_debug_args__);
    /// diff
    tt_byte_t diff = (tt_byte_t)((~(tt_size_t)data) & (align - 1) + 1);
    /// adjust the data
    data += diff;

    /// assert
    tt_assert(!((tt_size_t)data & (align - 1)));
    /// save diff
    data[-1] = diff;

    return (tt_pointer_t)data;
}

tt_pointer_t tt_allocator_align_malloc0_(tt_allocator_ref_t allocator, tt_size_t size, tt_size_t align __tt_debug_decl__)
{
    tt_pointer_t data = tt_null;

    data = tt_allocator_align_malloc_(allocator, size, align __tt_debug_args__);
    if(data) tt_memset(data, 0, size);

    return data;
}

tt_pointer_t tt_allocator_align_nalloc_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t align __tt_debug_decl__)
{
    return tt_allocator_align_malloc_(allocator, item * size, align __tt_debug_args__);
}

tt_pointer_t tt_allocator_align_nalloc0_(tt_allocator_ref_t allocator, tt_size_t item, tt_size_t size, tt_size_t align __tt_debug_decl__)
{
    return tt_allocator_align_malloc0_(allocator, item * size, align __tt_debug_args__);
}

tt_pointer_t tt_allocator_align_ralloc_(tt_allocator_ref_t allocator, tt_pointer_t data, tt_size_t size, tt_size_t align __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator);
    tt_assertf(!(align & 3), "invalid align size(%lu)", align);

    tt_byte_t diff = 0;
    tt_byte_t* data_new = tt_null;
    /// realloc
    if(data)
    {
        tt_assertf(!((tt_size_t)data & (align - 1)), "invalid align address(%p)", data);

        diff = ((tt_byte_t *)data)[-1];
        data = (tt_byte_t *)data - diff;
        data_new = (tt_byte_t*)tt_allocator_ralloc_(allocator, data, size + align __tt_debug_args__);
        tt_assert(data_new);
    }
    else //no data
    {
        data_new = (tt_byte_t*)tt_allocator_malloc_(allocator, size + align __tt_debug_args__);
    }
    /// diff
    diff = (tt_byte_t)((~(tt_size_t)data) & (align - 1) + 1);
    /// adjust the data
    data_new = data_new + diff;

    /// assert
    tt_assert(!((tt_size_t)data_new & (align - 1)));
    /// save diff
    data_new[-1] = diff;

    return (tt_pointer_t)data_new;
}

tt_bool_t tt_allocator_align_free_(tt_allocator_ref_t allocator, tt_pointer_t data __tt_debug_decl__)
{
    /// assert
    tt_assert(allocator && data);

    /// adjust data with diff
    tt_byte_t diff = ((tt_byte_t *)data)[-1];
    data = (tt_byte_t *)data - diff;

    return tt_allocator_free_(allocator, data __tt_debug_args__);
}

tt_void_t tt_allocator_clear(tt_allocator_ref_t allocator)
{
    /// assert
    tt_assert(allocator);

    /// entry
    tt_bool_t lock = allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK;
    if(!lock) tt_spinlock_entry(&allocator->lock);

    /// clear it
    if(allocator->clear) allocator->clear(allocator);

    if(!lock) tt_spinlock_leave(&allocator->lock);
}

tt_void_t tt_allocator_exit(tt_allocator_ref_t allocator)
{
    /// assert
    tt_assert(allocator);

    /// clear it before
    tt_allocator_clear(allocator);

    if(allocator->exit) allocator->exit(allocator);
}
