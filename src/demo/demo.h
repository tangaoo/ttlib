/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       demo.h
 * @ingroup    demo
 * @author     tango
 * @date       2020-11-19 
 * @brief      demo.h file
 */

#ifndef TT_DEMO_DEMO_H
#define TT_DEMO_DEMO_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define TT_DEMO_MAIN_DECL(name) tt_int_t tt_demo_##name##_main(tt_int_t argc, tt_char_t** argv) 

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

TT_DEMO_MAIN_DECL(utils);
TT_DEMO_MAIN_DECL(utils_dump);
TT_DEMO_MAIN_DECL(utils_mix);
TT_DEMO_MAIN_DECL(circular_buffer);
TT_DEMO_MAIN_DECL(single_list_entry);
TT_DEMO_MAIN_DECL(crc8);
TT_DEMO_MAIN_DECL(queue_entry);
TT_DEMO_MAIN_DECL(static_fixed_pool);
TT_DEMO_MAIN_DECL(fixed_pool);
TT_DEMO_MAIN_DECL(static_large_allocator);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__


#endif
