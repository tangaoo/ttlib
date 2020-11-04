/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       allocator.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-11 
 * @brief      allocator.c file
 */

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
    if(!(allocator->flag & TT_ALLOCATOR_FLAG_NOLOCK))
        tt_spinlock_entry(allocator->lock);
    
    /// malloc it
    
}