/*
 * @Copyright (C) 2019-2021, TTLIB
 * @file       uint8.c
 * @ingroup    container
 * @author     tango
 * @date       2020-11-19 
 * @brief      uint8.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "element.h"
#include "../../platform/platform.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * private interfaces
 */
static tt_size_t tt_element_uint8_hash(tt_element_ref_t e, tt_cpointer_t data, tt_size_t mask, tt_size_t index)
{
    return 0; /// TODO
}

static tt_long_t tt_element_uint8_comp(tt_element_ref_t e, tt_cpointer_t ldata, tt_cpointer_t rdata)
{
    return ((tt_p2u8(ldata) < tt_p2u8(rdata))? -1 : (tt_p2u8(ldata) > tt_p2u8(rdata))); ///TODO
}

static tt_pointer_t tt_element_uint8_data(tt_element_ref_t e, tt_cpointer_t buff)
{
    /// check
    tt_assert_and_check_return_val(buff, tt_null);

    return tt_u2p(*((tt_uint8_t *)buff));
}

tt_char_t const * tt_element_uint8_cstr(tt_element_ref_t e, tt_cpointer_t data, tt_char_t *str, tt_size_t maxn)
{
    /// check
    tt_assert_and_check_return_val(e && str, "");  

    tt_long_t n = snprintf(str, maxn, "%u", (tt_uint8_t)(tt_size_t)data);  
    if(n >= 0 && n < maxn) str[n] = '\0';   

    return (tt_char_t const *)str;
}

tt_void_t tt_element_uint8_free(tt_element_ref_t e, tt_pointer_t buff)
{
    /// check
    tt_assert_and_check_return(buff);

    *((tt_uint8_t *)buff) = 0;
}

tt_void_t tt_element_uint8_copy(tt_element_ref_t e, tt_pointer_t buff, tt_cpointer_t data)
{
    /// check
    tt_assert_and_check_return(buff);

    *((tt_uint8_t *)buff) = tt_p2u8(data);
}

tt_void_t tt_element_uint8_nfree(tt_element_ref_t e, tt_pointer_t buff, tt_size_t size)
{
    /// check
    tt_assert_and_check_return(buff);

    if(size) tt_memset((tt_uint8_t *)buff, 0, size);
}

tt_void_t tt_element_uint8_ncopy(tt_element_ref_t e, tt_pointer_t buff, tt_cpointer_t data, tt_size_t size)
{
    /// check
    tt_assert_and_check_return(buff);

    if(size) tt_memset((tt_uint8_t *)buff, tt_p2u8(data), size);   
}

/* //////////////////////////////////////////////////////////////////////////////////////
 * interfaces
 */
tt_element_t        tt_element_uint8(tt_noarg_t)
{
    tt_element_t e = {0};

    e.type   = TT_ELEMENT_TYPE_UINT8;
    e.flag   = 0;
    e.size   = sizeof(tt_uint8_t);
    e.hash   = tt_element_uint8_hash;
    e.comp   = tt_element_uint8_comp;
    e.data   = tt_element_uint8_data;
    e.cstr   = tt_element_uint8_cstr;
    e.free   = tt_element_uint8_free;
    e.dupl   = tt_element_uint8_copy;
    e.repl   = tt_element_uint8_copy;
    e.copy   = tt_element_uint8_copy;
    e.nfree  = tt_element_uint8_nfree;
    e.ndupl  = tt_element_uint8_ncopy;
    e.nrepl  = tt_element_uint8_ncopy;
    e.ncopy  = tt_element_uint8_ncopy;

    return e;
}