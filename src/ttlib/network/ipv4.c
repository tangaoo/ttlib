/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       ipv4.c
 * @ingroup    network
 * @author     tango
 * @date       2021-01-29 
 * @brief      ipv4.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * trace
 */
#define TT_TRACE_MODULE_NAME        "ipv4"
#define TT_TRACE_MODULE_DEBUG       (0) 

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "ipv4.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * implementation
 */
tt_void_t tt_ipv4_clear(tt_ipv4_ref_t ipv4)
{
    // check
    tt_assert_and_check_return(ipv4);

    // clear it
    ipv4->u32 = 0;
}

tt_bool_t tt_ipv4_is_any(tt_ipv4_ref_t ipv4)
{
    // check
    tt_assert_and_check_return_val(ipv4, tt_false);

    // is empty
    return !ipv4->u32;
}

tt_bool_t tt_ipv4_is_loopback(tt_ipv4_ref_t ipv4)
{
    // check
    tt_assert_and_check_return_val(ipv4, tt_false);

    // is loopback
    return (ipv4->u32 == 0x0100007f);
}

tt_bool_t tt_ipv4_is_equal(tt_ipv4_ref_t ipv4, tt_ipv4_ref_t other)
{
    // check
    tt_assert_and_check_return_val(ipv4 && other, tt_false);

    // is equal
    return (ipv4->u32 == other->u32);
}

tt_char_t const* tt_ipv4_cstr(tt_ipv4_ref_t ipv4, tt_char_t* data, tt_size_t maxn)
{
    // check
    tt_assert_and_check_return_val(ipv4 && data && maxn >= TT_IPV4_CSTR_MAXN, tt_null);

    // done
    tt_size_t size = snprintf(data, maxn - 1, "%u.%u.%u.%u", ipv4->u8[0], ipv4->u8[1], ipv4->u8[2], ipv4->u8[3]);
    data[size] = '\0';

    // ok
    return data;
}

tt_bool_t tt_ipv4_cstr_set(tt_ipv4_ref_t ipv4, tt_char_t const* cstr)
{
    // check
    tt_assert_and_check_return_val(ipv4 && cstr, tt_null);

    // done
    

}