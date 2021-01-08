/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       mix.c
 * @ingroup    utils
 * @author     tango
 * @date       2020-11-19 
 * @brief      mix.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "mix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_int_t tt_little_endian(tt_void_t)
{
    tt_uint16_t data   = 0x1234;
    tt_byte_t const* p = (tt_byte_t const*)&data;

    // done
    return (p[0] == 0x34 && p[1] == 0x12)? TT_LITTLE_ENDIAN: TT_BIG_ENDIAN;
}