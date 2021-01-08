/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       native_memory.c
 * @ingroup    platform
 * @author     tango
 * @date       2020-11-19 
 * @brief      native_memory.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "native_memory.h"
#include "stdlib.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_bool_t tt_native_memory_init(tt_void_t)
{
    return tt_true;
}

tt_void_t tt_native_memory_exit(tt_void_t)
{}

tt_pointer_t tt_native_memory_malloc(tt_size_t size)
{
    /// check
    tt_check_return_val(size, tt_null);
    return malloc(size);
}

tt_pointer_t tt_native_memory_malloc0(tt_size_t size)
{
    /// check
    tt_check_return_val(size, tt_null);
    return calloc(1, size);
}

tt_pointer_t tt_native_memory_nalloc(tt_size_t item, tt_size_t count)
{
    /// check
    tt_check_return_val(item && count, tt_null);
    return malloc(item * count);
}

tt_pointer_t tt_native_memory_nalloc0(tt_size_t item, tt_size_t count)
{
    /// check
    tt_check_return_val(item && count, tt_null);
    return calloc(1, item * count);
}

tt_pointer_t tt_native_memory_realloc(tt_pointer_t data, tt_size_t size)
{
    /// check
    if(!size)
    {
        free(data);
        return tt_null;
    }
    else if(data == tt_null)
        return malloc(size);
    else
        return realloc(data, size);
}

tt_bool_t tt_native_memory_free(tt_pointer_t data)
{
    tt_check_return_val(data, tt_false);

    free(data);
    return tt_true;
}