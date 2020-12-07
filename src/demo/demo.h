/*
 * @Copyright (C) 2019-2020, TTLIB
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
#include "utils/demo_utils_mix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#define TT_DEMO_DECL(name) tt_int_t tt_demo_##name##_main(tt_int_t argc, tt_char_t** argv) 

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

TT_DEMO_DECL(utils);
TT_DEMO_DECL(circular_buffer);
TT_DEMO_DECL(single_list_entry);
TT_DEMO_DECL(crc8);
TT_DEMO_DECL(queue_entry);
TT_DEMO_DECL(static_fixed_pool);
TT_DEMO_DECL(utils_dump);

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__


#endif
