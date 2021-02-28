/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       data.c
 * @ingroup    steam
 * @author     tango
 * @date       2021-01-10 
 * @brief      data.c file
 */

#if 0

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "prefix.h"
#include "../../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the data stream type
typedef struct __tt_stream_data_t
{
    // the data
    tt_byte_t*          data;

    // the head
    tt_byte_t*          head;

    // the size
    tt_size_t           size;

    // the data be referenced?
    tt_bool_t           bref;

}tt_stream_data_t;

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
static __tt_inline__ tt_stream_data_t* tt_stream_data_cast(tt_stream_ref_t stream)
{
    return stream;
}

static tt_bool_t tt_stream_data_open(tt_stream_ref_t stream)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data && stream_data->data && stream_data->size, tt_false);

    // init head
    stream_data->head = stream_data->data;

    // ok
    return tt_true;
}

static tt_bool_t tt_stream_data_clos(tt_stream_ref_t stream)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data, tt_false);

    // clear head
    stream_data->head = tt_null;

    // ok
    return tt_true;
}

static tt_void_t tt_stream_data_exit(tt_stream_ref_t stream)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data, tt_false);

    // clear head
    stream_data->head = tt_null;

    // exit data
    if(stream_data->data && !stream_data->bref) tt_free(stream_data->data);
    stream_data->data = tt_null;
    stream_data->size = 0;

}

static tt_long_t tt_stream_data_read(tt_stream_ref_t stream, tt_byte_t* data, tt_size_t size)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data && stream_data->data && stream_data->head, -1);

    // check
    tt_check_return_val(data, -1);
    tt_check_return_val(size, 0);

    do
    {
        // the left
        tt_size_t left = stream_data->size - (stream_data->head - stream_data->data);

        // the need
        tt_size_t need = tt_min(left, size);

        // read
        if (need) tt_memcpy(data, stream_data->head, need);

        // update head
        stream_data->head += need;

    } while (0);

    // ok
    return (tt_long_t)need;

}

static tt_long_t tt_stream_data_writ(tt_stream_ref_t stream, tt_byte_t* data, tt_size_t size)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data && stream_data->data && stream_data->head, -1);

    // check
    tt_check_return_val(data, -1);
    tt_check_return_val(size, 0);

    do
    {
        // the left
        tt_size_t left = stream_data->size - (stream_data->head - stream_data->data);

        // the need
        tt_size_t need = tt_min(left, size);

        // read
        if (need) tt_memcpy(stream_data->head, data, need);

        // update head
        stream_data->head += need;

    } while (0);

    // ok
    return left? (tt_long_t)need : -1;
}

static tt_bool_t tt_stream_data_seek(tt_stream_ref_t stream, tt_size_t offset)
{
    // check
    tt_stream_data_t* stream_data = tt_stream_data_cast(stream);
    tt_assert_and_check_return_val(stream_data && offset <= stream_data->size, -1);

    // seek
    stream_data->head = stream_data->data + offset;

    // ok
    return tt_true;
}

#endif