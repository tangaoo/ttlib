/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       single_list_entry.c
 * @ingroup    container
 * @author     tango
 * @date       2020-11-19 
 * @brief      single_list_entry.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
* include
*/
#include "single_list_entry.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * itor implementation
 */
static tt_size_t tt_single_list_entry_itor_size_(tt_iterator_ref_t iterator)
{
	tt_assert(iterator);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);
	
	/// size
	return list->size;
}

static tt_size_t tt_single_list_entry_itor_head_(tt_iterator_ref_t iterator)
{
	tt_assert(iterator);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);

	/// list head entry
	return (tt_size_t)list->next;
}

static tt_size_t tt_single_list_entry_itor_last_(tt_iterator_ref_t iterator)
{
	tt_assert(iterator);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);

	// list last entry
	return (tt_size_t)list->last;
}

static tt_size_t tt_single_list_entry_itor_tail_(tt_iterator_ref_t iterator)
{
	tt_assert(iterator);

	return (tt_size_t)0;
}

static tt_size_t tt_single_list_entry_itor_next_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(itor);

	/// next
	return (tt_size_t)((tt_single_list_entry_ref_t)itor)->next;
}

static tt_pointer_t tt_single_list_entry_itor_item_(tt_iterator_ref_t iterator, tt_size_t itor)
{
	tt_assert(iterator && itor);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);

	/// item
	return (tt_pointer_t)(itor - list->eoffset);
}

static tt_void_t tt_single_list_entry_itor_copy_(tt_iterator_ref_t iterator, tt_size_t itor, tt_cpointer_t item)
{
	tt_assert(iterator && itor && item);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);

	/// copy
	list->copy((tt_pointer_t)(itor - list->eoffset), (tt_pointer_t)item);	
}

static tt_void_t tt_single_list_entry_itor_nremove_(tt_iterator_ref_t iterator, tt_size_t prev, tt_size_t next, tt_size_t size)
{
	tt_assert(iterator && prev && next && size);

	tt_single_list_entry_head_ref_t list = tt_container_of(tt_single_list_entry_head_t, itor, iterator);
	tt_assert(list);

	tt_single_list_entry_ref_t prev_entry = (tt_single_list_entry_ref_t)prev;
	tt_single_list_entry_ref_t next_entry = (tt_single_list_entry_ref_t)next;

	/// remove entris
	prev_entry->next = next_entry;
	/// update size
	list->size -= size;
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_iterator_ref_t tt_single_list_entry_iterator(tt_single_list_entry_head_ref_t list)
{
	tt_assert(list);

	/// the iterator
	return &list->itor;
}

tt_void_t tt_single_list_entry_init_(tt_single_list_entry_head_ref_t list, tt_size_t entry_offset, tt_size_t entry_size, tt_entry_copy_t copy_func)
{
	/// assert
	tt_assert(list && entry_size > sizeof(tt_single_list_entry_t));
	
	/// init list
	list->next = tt_null;
	list->last = tt_null;
	list->size = 0;
	list->eoffset = entry_offset;
	list->copy = copy_func;

	static tt_iterator_op_t op =
	{
		tt_single_list_entry_itor_size_
	,   tt_single_list_entry_itor_head_
	,   tt_single_list_entry_itor_last_
	,   tt_single_list_entry_itor_tail_
	,   tt_null
	,   tt_single_list_entry_itor_next_
	,   tt_single_list_entry_itor_item_
	,   tt_null
	,   tt_single_list_entry_itor_copy_
	,   tt_null
	,   tt_single_list_entry_itor_nremove_
};
	/// init itor
	list->itor.op = &op;
	list->itor.mode = TT_ITERATOR_MODE_FORWARD;
	list->itor.priv = tt_null;
	list->itor.step = entry_size;
	
}

tt_void_t tt_single_list_entry_exit(tt_single_list_entry_head_ref_t list)
{
	tt_assert(list);

	/// exit
	list->next = tt_null;
	list->last = tt_null;
	list->size = 0;
}