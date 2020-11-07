/*
 * @Copyright (C)  2020  .Tango. all right reserved
 * @file       queue_entry.h
 * @ingroup    container
 * @author     tango
 * @date       2020-11 
 * @brief      queue_entry.h file
 */

#ifndef  TT_CONTAINER_QUEUE_ENTRY_H
#define  TT_CONTAINER_QUEUE_ENTRY_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "list_entry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_enter__

/*! init the list entry
*
* @code
*
// the xxxx entry type
typedef struct __tt_xxxx_entry_t
{
// the list entry
tt_list_entry_t      entry;

// the data
tt_size_t                   data;

}tt_xxxx_entry_t;


// init the list
tt_single_list_entry_head_t list;
tt_single_list_entry_init(&list, tt_xxxx_entry_t, entry, tt_xxxx_entry_copy);

* @endcode
*/
#define tt_queue_entry_init(list, type, entry)     tt_single_list_entry_init_(list, tt_offsetof(type, entry), sizeof(type), tt_null)
#define tt_queue_entry_exit(list)                  tt_single_list_entry_exit(list)

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef tt_single_list_entry_t                     tt_queue_entry_t;
typedef tt_queue_entry_t*                          tt_queue_entry_ref_t;
typedef tt_single_list_entry_head_t                tt_queue_entry_head_t;
typedef tt_queue_entry_head_t*                     tt_queue_entry_head_ref_t;


/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */

/*! queue clear
 *
 * @param q                           the queue
 */
static __tt_inline__ tt_void_t        tt_queue_entry_clear(tt_queue_entry_head_ref_t q)
{
    tt_single_list_entry_clear(q);
}

/*! get queue size
 *
 * @param a                           the queue
 * @return                            the count of queue
 */
static __tt_inline__ tt_size_t        tt_queue_entry_size(tt_queue_entry_head_ref_t q)
{
    return tt_single_list_entry_size(q);
}

/*! put in queue 
*
* @param a                            the queue
* @param entry                        need to be inserted
*/
static __tt_inline__ tt_void_t        tt_put_queue_entry(tt_queue_entry_head_ref_t q, tt_queue_entry_ref_t entry)
{
    tt_single_list_entry_insert_head(q, entry);
}

/*! get out queue 
*
* @param a                            the queue
* @param entry                        need to be inserted
*/
static __tt_inline__ tt_void_t        tt_get_queue_entry(tt_queue_entry_head_ref_t q)
{
    tt_single_list_entry_insert_head(q, entry);
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * extern
 */
__tt_extern_c_leave__

#endif