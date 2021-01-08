/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       iterator.c
 * @ingroup    container
 * @author     tango
 * @date       2020-11-19 
 * @brief      iterator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "iterator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
* implementation
*/
tt_size_t tt_iterator_mode(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator);
	
	return iterator->mode;
}

tt_size_t tt_iterator_step(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator);

	return iterator->step;
}

tt_size_t tt_iterator_size(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->size);

	return iterator->op->size(iterator);
}

tt_size_t tt_iterator_head(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->head);

	return iterator->op->head(iterator);
}

tt_size_t tt_iterator_last(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->last);

	return iterator->op->last(iterator);
}

tt_size_t tt_iterator_tail(tt_iterator_ref_t iterator)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->tail);

	return iterator->op->tail(iterator);
}

tt_size_t tt_iterator_prev(tt_iterator_ref_t iterator, tt_size_t itor)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->prev);

	return iterator->op->prev(iterator, itor);
}

tt_size_t tt_iterator_next(tt_iterator_ref_t iterator, tt_size_t itor)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->next);

	return iterator->op->next(iterator, itor);
}

tt_pointer_t tt_iterator_item(tt_iterator_ref_t iterator, tt_size_t itor)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->item);

	return iterator->op->item(iterator, itor);
}

tt_long_t tt_iterator_comp(tt_iterator_ref_t iterator, tt_cpointer_t litem, tt_cpointer_t ritem)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->comp);

	return iterator->op->comp(iterator, litem, ritem);
}

tt_void_t tt_iterator_copy(tt_iterator_ref_t iterator, tt_size_t itor, tt_cpointer_t item)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->copy);

	iterator->op->copy(iterator, itor, item);
}

tt_void_t tt_iterator_remove(tt_iterator_ref_t iterator, tt_size_t itor)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->remove);

	iterator->op->remove(iterator, itor);
}

tt_void_t tt_iterator_nremove(tt_iterator_ref_t iterator, tt_size_t prev, tt_size_t next, tt_size_t size)
{
	/// check
	tt_assert(iterator && iterator->op && iterator->op->nremove);

	iterator->op->nremove(iterator, prev, next, size);
}






