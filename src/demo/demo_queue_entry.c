/*
 * @Copyright (C)  2020  .Tango. all right reserved
 * @file       demo_queue_entry.c
 * @ingroup    demo
 * @author     tango
 * @date       2020-11 
 * @brief      demo_queue_entry.c file
 */

#define TT_TRACE_MODULE_NAME    "queue_entry_demo"
#define TT_TRACE_DEBUG          1

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
//#include "ttlib.h"
#include "../ttlib/ttlib.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef struct __tt_demo_queue_entry_t{
    tt_queue_entry_t entry;
    tt_int_t         data;
}tt_demo_queue_entry_t, *tt_demo_queue_entry_ref_t;

tt_demo_queue_entry_t demo_queue[5] = {
    {{0, 0}, 0},
    {{0, 0}, 1},
    {{0, 0}, 2},
    {{0, 0}, 3},
    {{0, 0}, 4},
};
tt_queue_entry_head_t q = {0};

tt_void_t tt_demo_queue_entry_main(tt_void_t)
{
    tt_demo_queue_entry_ref_t queue_tmp = tt_null;
    /// init queue
    tt_queue_entry_init(&q, tt_demo_queue_entry_t, entry);

    /// put queue
    tt_queue_entry_put(&q, &demo_queue[0].entry);
    tt_queue_entry_put(&q, &demo_queue[1].entry);
    tt_queue_entry_put(&q, &demo_queue[2].entry);

    /// get queue
    for(tt_int_t i = 0; i < 3; i++)
    {
        queue_tmp = (tt_demo_queue_entry_ref_t)tt_queue_entry(&q, tt_queue_entry_get(&q));
        tt_trace_d("%d", queue_tmp->data );
    }

}

