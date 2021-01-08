/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       color.h
 * @ingroup    demo
 * @author     tango
 * @date       2020-12-20 
 * @brief      color.h file
 */
#ifndef TT_DEMO_COLOR_H
#define TT_DEMO_COLOR_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ttlib.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

// color
enum
{
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    AQUA = 3,
    RED = 4,
    PURPLE = 5,
    YELLOW = 6,
    WHITE = 7,
    GRAY = 8,

    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_AQUA = 11,
    LIGHT_RED = 12,
    LIGHT_PURPLE = 13,
    LIGHT_YELLOW = 14,
    LIGHT_WHITE = 15,

    DEFAULT = 16,
};

static const tt_char_t *colors[] = {
    "\x1B[0m",
    "\x1B[34m",
    "\x1B[32m",
    "\x1B[36m",
    "\x1B[31m",
    "\x1B[35m",
    "\x1B[33m",
    "\x1B[37m",
    "",
    "\x1B[34m",
    "\x1B[32m",
    "\x1B[36m",
    "\x1B[31m",
    "\x1B[35m",
    "\x1B[33m",
    "\x1B[37m",
    "\x1B[39m",
};

static __tt_inline__ tt_void_t tt_set_color(tt_int_t color) 
{
    printf("%s", colors[color]);
}

static __tt_inline__ tt_void_t tt_set_color_default(tt_void_t) 
{
    tt_set_color(DEFAULT);
}

static __tt_inline__ tt_void_t tt_print_title(tt_char_t* title)
{
    puts("\n----------------------------------------------");

    tt_set_color(GREEN);
    printf("  %s\n", title);
    tt_set_color_default();

    puts("----------------------------------------------");
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif