/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       list_entry.c
 * @ingroup    container
 * @author     tango
 * @date       2020-11-19 
 * @brief      list_entry.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "list_entry.h"
#include "../memory/impl/static_fixed_pool.h"

/* //////////////////////////////////////////////////////////////////////////////////////
* iterator implementation
*/
static tt_size_t tt_list_entry_itor_size_(tt_iterator_ref_t iterator)
{
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	return list->size;
}

static tt_size_t tt_list_entry_itor_head_(tt_iterator_ref_t iterator)
{
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	return (tt_size_t)list->next;
}

static tt_size_t tt_list_entry_itor_last_(tt_iterator_ref_t iterator)
{
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	return (tt_size_t)list->prev;
}

static tt_size_t tt_list_entry_itor_tail_(tt_iterator_ref_t iterator)
{
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	return (tt_size_t)list;
}

static tt_size_t tt_list_entry_itor_prev_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(itor);

	return (tt_size_t)((tt_list_entry_ref_t)itor)->prev;
}

static tt_size_t tt_list_entry_itor_next_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(itor);

	return (tt_size_t)((tt_list_entry_ref_t)itor)->next;
}

static tt_pointer_t tt_list_entry_itor_item_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(iterator && itor);
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	/// data
	return (tt_pointer_t)(itor - list->eoffset);
}

static tt_void_t tt_list_entry_itor_copy_(tt_iterator_ref_t iterator, tt_size_t itor, tt_cpointer_t item)
{
	tt_assert(iterator && itor && item);
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list && list->copy);
	tt_assert(list->eoffset && item);

	list->copy((tt_pointer_t)(itor - list->eoffset), (tt_pointer_t)item);
}

static tt_void_t tt_list_entry_itor_remove_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(iterator && itor);
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list);

	tt_list_entry_remove(list, (tt_list_entry_ref_t)itor);
}

static tt_void_t tt_list_entry_itor_nremove_(tt_iterator_ref_t iterator, tt_size_t prev, tt_size_t next, tt_size_t size)
{
	// check
	tt_list_entry_head_ref_t list = tt_container_of(tt_list_entry_head_t, itor, iterator);
	tt_assert(list && prev && next);

	// no size?
	tt_check_return(size);

	// the entry
	tt_list_entry_ref_t prev_entry = (tt_list_entry_ref_t)prev;
	tt_list_entry_ref_t next_entry = (tt_list_entry_ref_t)next;

	// remove entries
	prev_entry->next = next_entry;
	next_entry->prev = prev_entry;

	// update size
	list->size -= size;
}

/* //////////////////////////////////////////////////////////////////////////////////////
* implementation
*/
tt_iterator_ref_t tt_list_entry_iterator(tt_list_entry_head_ref_t list)
{
	/// assert
	tt_assert(list);

	return &list->itor;
}

tt_void_t tt_list_entry_init_(tt_list_entry_head_ref_t list, tt_size_t entry_offset, tt_size_t entry_size, tt_entry_copy_t copy_func)
{
	/// assert
	tt_assert(list && entry_size > sizeof(tt_list_entry_t));

	/// init list
	list->next = (tt_list_entry_ref_t)list;
	list->prev = (tt_list_entry_ref_t)list;
	list->size = 0;
	list->eoffset = entry_offset;
	list->copy = copy_func;

	/// init op
	static tt_iterator_op_t op = 
	{
		tt_list_entry_itor_size_
	,   tt_list_entry_itor_head_
	,   tt_list_entry_itor_last_
	,   tt_list_entry_itor_tail_
	,   tt_list_entry_itor_prev_
	,   tt_list_entry_itor_next_
	,   tt_list_entry_itor_item_
	,   tt_null
	,   tt_list_entry_itor_copy_
	,   tt_list_entry_itor_remove_
	,   tt_list_entry_itor_nremove_
	};

	list->itor.op   = &op;
	list->itor.mode = TT_ITERATOR_MODE_FORWARD | TT_ITERATOR_MODE_REVERSE;
	list->itor.priv = tt_null;
	list->itor.step = entry_size;

}

tt_void_t tt_list_entry_exit(tt_list_entry_head_ref_t list)
{
	/// assert 
	tt_assert(list);

	list->next = (tt_list_entry_ref_t)list;
	list->prev = (tt_list_entry_ref_t)list;
	list->size = 0;
}