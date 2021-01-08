/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       trace.c
 * @ingroup    utils
 * @author     tango
 * @date       2020-11-19 
 * @brief      trace.c file
 */

#define _CRT_SECURE_NO_WARNINGS

/*//////////////////////////////////////////////////////////////////////////////////////
* includes 
*/
#include "trace.h"
#include "../platform/platform.h"
#include <stdio.h>
#include <stdarg.h>

#ifdef TT_COMPILER_IS_MSVC
#	define snprintf _snprintf   // >=C99 support 'snprintf'
#endif

#define TT_TRACE_LINE_SIZE      1024

/*//////////////////////////////////////////////////////////////////////////////////////
* globals
*/
static tt_char_t g_line[TT_TRACE_LINE_SIZE];
static tt_trace_mode_e g_trace_mode = TT_TRACE_MODE_PRINT;
static tt_mutex_t g_mutex_trace;
static tt_mutex_ref_t g_mutex_trace_ref = &g_mutex_trace;

/* //////////////////////////////////////////////////////////////////////////////////////
* implementation
*/

tt_bool_t tt_trace_init(tt_void_t)
{
	return tt_null != tt_mutex_init_impl(&g_mutex_trace) ? tt_true : tt_false;
}

tt_bool_t tt_trace_exit(tt_void_t)
{
	tt_mutex_exit_impl(&g_mutex_trace);
	return tt_true;
}

tt_bool_t tt_trace_mode_set(tt_trace_mode_e mode)
{
	// mutex enter
	tt_mutex_entry(g_mutex_trace_ref);

	g_trace_mode = mode;

	// mutex leave
	tt_mutex_leave(g_mutex_trace_ref);

	return tt_true;
}

tt_trace_mode_e tt_trace_mode(tt_void_t)
{

	return g_trace_mode;
}

tt_void_t tt_trace_done_with_args(tt_char_t const* prefix, tt_char_t const* module, tt_char_t const* format, va_list args)
{
	// check
	// mutex enter
	tt_mutex_entry(g_mutex_trace_ref);

	do
	{
		tt_char_t *p = g_line;
		// end
		tt_char_t *e = p + TT_TRACE_LINE_SIZE;
		// start
		tt_char_t *s = g_line;

		if (prefix && p < e) p += snprintf(p, e - p, "[%s]: ", prefix);
		if (module && p < e) p += snprintf(p, e - p, "[%s]: ", module);
		if (p < e) p += vsnprintf(p, e - p, format, args);

		// append end
		if (p < e) *p = '\0';
		e[-1] = '\0';

		// deal it
		if (g_trace_mode == TT_TRACE_MODE_PRINT) printf("%s", s);	

	} while (0);

	// mutex leave
	tt_mutex_leave(g_mutex_trace_ref);
	
}

tt_void_t tt_trace_done(tt_char_t const* prefix, tt_char_t const* module, tt_char_t const* format, ...)
{
	//check format

	va_list args;
	va_start(args, format);

	tt_trace_done_with_args(prefix, module, format, args);
	va_end(args);
}

