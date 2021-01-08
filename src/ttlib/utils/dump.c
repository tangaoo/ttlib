/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       dump.c
 * @ingroup    utils
 * @author     tango
 * @date       2020-11 
 * @brief      dump.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "dump.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

/// memory dump
tt_void_t tt_memory_dump(tt_pointer_t ptr, tt_size_t byte_cnt) 
{
    // check
    tt_assert_and_check_return(ptr && byte_cnt);

    tt_size_t i;

    // print address
    tt_trace_raw("0X%08X    ", (tt_size_t)ptr);

    for (i = 0; i < byte_cnt; i++) 
    {
        // print " ", per 8 bytes
        if (i % 8 == 0 && i != 0) tt_trace_raw(" ");
        if (i % 16 == 0 && i != 0)
        {
            // print "\n", per 16 bytes
            tt_trace_raw("\n");
            
            // print address
            tt_trace_raw("0X%08X    ", (tt_size_t)ptr + i);
        }
            
        tt_trace_raw("%02x ", *((tt_byte_t*)ptr + i));
    }

    tt_trace_raw("\n");

}