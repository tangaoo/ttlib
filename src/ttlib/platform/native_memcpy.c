/*
 * @Copyright (C)  2020  .Harman. all right reserved
 * @file       native_memcpy.c
 * @ingroup    platform
 * @author     tango
 * @date       2020-11 
 * @brief      native_memcpy.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "native_memcpy.h"
#include "stdio.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_void_t tt_memcpy(tt_pointer_t dst, tt_cpointer_t src, tt_size_t size)
{
    memcpy(dst, src, size);
}

tt_void_t tt_memmov(tt_pointer_t dst, tt_pointer_t src, tt_size_t size)
{
    memmove(dst, src, size);
}

tt_void_t tt_memset(tt_pointer_t dst, tt_int32_t ch, tt_size_t size)
{
    memset(dst, ch, size);
}