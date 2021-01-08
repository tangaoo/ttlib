/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       circular_buffer.c
 * @ingroup    buffer
 * @author     tango
 * @date       2020-11-19 
 * @brief      circular_buffer.c file
 */

/*//////////////////////////////////////////////////////////////////////////////////////
* includes
*/
#include "circular_buffer.h"
#include "string.h"

/// cb is full?
static tt_bool_t tt_circular_buffer_is_full(tt_circular_buffer_ref_t buff)
{
	return (buff->w_idx - buff->r_idx == buff->size) ? tt_true : tt_false;
}

/// cb is empty?
static tt_bool_t tt_circular_buffer_is_empty(tt_circular_buffer_ref_t buff)
{
	return (buff->r_idx == buff->w_idx) ? tt_true : tt_false;
}

/// cb have used
static tt_uint64_t tt_circular_buffer_used(tt_circular_buffer_ref_t buff)
{
	/// assert
	tt_assert(buff->w_idx >= buff->r_idx);

	return (buff->w_idx - buff->r_idx);
}

/// cb buffer left
static tt_uint64_t tt_circular_buffer_left(tt_circular_buffer_ref_t buff)
{
	/// assert
	tt_assert((buff->w_idx - buff->r_idx) <= buff->size);

	return (buff->size - tt_circular_buffer_used(buff));
}

/*! assign tail size and head size for cb write
*
* @param buffer
* @param min_size, user ture buffer size for done
* @param tail, tail size of min_size (min_size = tail + head)
* @param head, head size of min_size (min_size = tail + head)
*/
static tt_bool_t tt_cirular_buffer_writ_assign(tt_circular_buffer_ref_t buff, tt_size_t min_size, tt_size_t* tail, tt_size_t* head)
{
	tt_assert(tail && head && (min_size <= buff->size));
	tt_check_return_val(min_size, tt_false);

	if ((buff->size - (buff->w_idx % buff->size)) >= min_size)
	{
		*tail = min_size;
		*head = 0;
	}
	else
	{
		*tail = buff->size - (buff->w_idx % buff->size);
		*head = min_size - *tail;
	}

	return tt_true;
}

static tt_bool_t tt_cirular_buffer_read_assign(tt_circular_buffer_ref_t buff, tt_size_t min_size, tt_size_t* tail, tt_size_t* head)
{
	tt_assert(tail && head);
	tt_check_return_val(min_size, tt_false);

	if ((buff->size - (buff->r_idx % buff->size)) >= min_size)
	{
		*tail = min_size;
		*head = 0;
	}
	else
	{
		*tail = buff->size - (buff->r_idx % buff->size);
		*head = min_size - *tail;
	}

	return tt_true;
}

tt_bool_t tt_circular_buffer_init(tt_circular_buffer_ref_t buff, tt_byte_t *data, tt_size_t size)
{
	tt_check_return_val(data && size, tt_false);
	
	buff->r_idx = 0;
	buff->w_idx = 0;
	buff->data = data;
	buff->size = size;

	return tt_true;
}

tt_bool_t tt_circular_buffer_exit(tt_circular_buffer_ref_t buff)
{
	tt_assert(buff);

	buff->r_idx = 0;
	buff->w_idx = 0;
	buff->data = tt_null;
	buff->size = 0;

	return tt_true;
}

tt_uint64_t tt_circular_buffer_writ_cover(tt_circular_buffer_ref_t buff, tt_byte_t *in, tt_size_t size)
{
	/// assert
	tt_assert(in);
	tt_size_t tail_size = 0, head_size = 0, valid_size = 0;
	
	if (size > buff->size)
	{
		valid_size = buff->size + size % buff->size;

		/// 1th
		tt_cirular_buffer_writ_assign(buff, buff->size, &tail_size, &head_size);
		/// tail write done
		memcpy(buff->data + buff->w_idx % buff->size, &in[size - valid_size], tail_size);
		/// head write done
		if (head_size) memcpy(buff->data, &in[size - valid_size] + tail_size, head_size);

		/// 2th TODO need to opt
		tt_cirular_buffer_writ_assign(buff, size % buff->size, &tail_size, &head_size);
		/// tail write done
		memcpy(buff->data + buff->w_idx % buff->size, &in[size - size % buff->size], tail_size);
		/// head write done
		if (head_size) memcpy(buff->data, &in[size - size % buff->size] + tail_size, head_size);

		/// update index
		buff->w_idx += size;
		if (buff->w_idx - buff->r_idx > buff->size)
			buff->r_idx = buff->w_idx - buff->size;

	}
	else
	{
		tt_cirular_buffer_writ_assign(buff, size, &tail_size, &head_size);

		/// tail write done
		memcpy(buff->data + buff->w_idx % buff->size, in, tail_size);
		/// head write done
		if (head_size) memcpy(buff->data, in + tail_size, head_size);

		/// update index
		buff->w_idx += size;
		if (buff->w_idx - buff->r_idx > buff->size)
			buff->r_idx = buff->w_idx - buff->size;
	}
		
	return (size);
}

tt_uint64_t tt_circular_buffer_writ(tt_circular_buffer_ref_t buff, tt_byte_t *in, tt_size_t size)
{
	tt_assert(in);
	tt_size_t min_size = 0, tail_size = 0, head_size = 0;

	if (!tt_circular_buffer_is_full(buff))
	{
		min_size = tt_min(tt_circular_buffer_left(buff), size);
		tt_cirular_buffer_writ_assign(buff, min_size, &tail_size, &head_size);

		/// tail write done
		memcpy(buff->data + buff->w_idx % buff->size, in, tail_size);
		/// head write done
		if (head_size) memcpy(buff->data, in + tail_size, head_size);

		/// update index
		buff->w_idx += min_size;
	}
	else
	{
		tt_trace_w("circular buff full, stop write");
		return 0;  /// write failed!
	}

	return min_size;
}

tt_uint64_t tt_circular_buffer_read(tt_circular_buffer_ref_t buff, tt_byte_t *out, tt_size_t size)
{
	tt_assert(out);
	tt_size_t min_size = 0, tail_size = 0, head_size = 0;

	if (!tt_circular_buffer_is_empty(buff))
	{
		min_size = tt_min(tt_circular_buffer_used(buff), size);
		tt_cirular_buffer_read_assign(buff, min_size, &tail_size, &head_size);

		/// tail read done
		memcpy(out, buff->data + buff->r_idx % buff->size, tail_size);
		/// head read done
		if (head_size) memcpy(out + tail_size, buff->data, head_size);

		/// update index
		buff->r_idx += min_size;
	}
	else
	{
		tt_trace_w("circular buff empty, stop read");
		return 0;  /// read failed!
	}

	return min_size;
}
