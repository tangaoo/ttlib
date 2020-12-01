/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       single_list.h
 * @ingroup    container
 * @author     tango
 * @date       2020-11-19 
 * @brief      single_list.h file
 */

#ifndef TT_CONTAINER_SINGLE_LIST_H
#define TT_CONTAINER_SINGLE_LIST_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "element/element.h"
#include "iterator.h"

/*! the double list ref type
 *
 *
 * <pre>
 * list: |-----| => |-------------------------------------------------=> |------| => |------| => tail
 *        head                                                                         last       
 *
 * performance: 
 *
 * insert:
 * insert midd: slow
 * insert head: fast
 * insert tail: fast
 * insert next: fast
 *
 * remove:
 * remove midd: slow
 * remove head: fast
 * remove last: fast
 * remove next: fast
 *
 * iterator:
 * next: fast
 * </pre>
 *
 */

__tt_extern_c_enter__

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */
typedef tt_iterator_ref_t   tt_single_list_ref_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */

/*! init list
 *
 * @param grow          the grow size
 * @param element       the element
 *
 * @return              the list
 */
tt_single_list_ref_t    tt_single_list_init(tt_size_t grow, tt_element_t element);

/*! exit list
 *
 * @param list          the list
 */
tt_void_t               tt_single_list_exit(tt_single_list_ref_t list);

/*! clear list
 *
 * @param list          the list
 */
tt_void_t               tt_single_list_clear(tt_single_list_ref_t list);

/*! the list head item
 *
 * @param list          the list
 *
 * @return              the head item
 */
tt_pointer_t            tt_single_list_head(tt_single_list_ref_t list);

/*! the list last item
 *
 * @param list          the list
 *
 * @return              the last item
 */
tt_pointer_t            tt_single_list_last(tt_single_list_ref_t list);

/*! insert the next item
 *
 * @param list          the list
 * @param itor          the item itor
 * @param data          the item data
 *
 * @return              the item itor
 */
tt_size_t               tt_single_list_insert_next(tt_single_list_ref_t list, tt_size_t itor, tt_cpointer_t data);

/*! insert the head item
 *
 * @param list          the list
 * @param data          the item data
 *
 * @return              the item itor
 */
tt_size_t               tt_single_list_insert_head(tt_single_list_ref_t list, tt_cpointer_t data);

/*! insert the tail item
 *
 * @param list          the list
 * @param data          the item data
 *
 * @return              the item itor
 */
tt_size_t               tt_single_list_insert_tail(tt_single_list_ref_t list, tt_cpointer_t data);

/*! replace the item
 *
 * @param list          the list
 * @param itor          the item itor
 * @param data          the item data
 * 
 * @return              tt_void
 */
tt_void_t               tt_single_list_replace(tt_single_list_ref_t list, tt_size_t itor, tt_cpointer_t data);

/*! replace the head item
 *
 * @param list          the list
 * @param data          the item data
 * 
 * @return              tt_void
 */
tt_void_t               tt_single_list_replace_head(tt_single_list_ref_t list, tt_cpointer_t data);

/*! replace the last item
 *
 * @param list          the list
 * @param data          the item data
 * 
 * @return              tt_void
 */
tt_void_t               tt_single_list_replace_last(tt_single_list_ref_t list, tt_cpointer_t data);

/*! remove the next item
 *
 * @param list          the list
 * @param itor          the item itor
 * 
 * @return              tt_void_t
 */
tt_void_t               tt_single_list_remove_next(tt_single_list_ref_t list, tt_size_t itor);

/*! remove the head item
 *
 * @param list          the list
 * 
 * @return              tt_void_t
 */
tt_void_t               tt_single_list_remove_head(tt_single_list_ref_t list);

/*! the item count
 *
 * @param list          the list
 *
 * @return              the item count
 */
tt_size_t               tt_single_list_size(tt_single_list_ref_t list);

/*! the item maxn count 
 *
 * @param list          the list
 *
 * @return              the item maxn count
 */
tt_size_t               tt_single_list_maxn(tt_single_list_ref_t list);


__tt_extern_c_leave__

#endif
