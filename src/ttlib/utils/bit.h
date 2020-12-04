/*
 * @Copyright (C) 2019-2020, TTLIB
 * @file       bit.h
 * @ingroup    utils
 * @author     tango
 * @date       2020-11-19 
 * @brief      bit.h file
 */

#ifndef TT_UTILS_BIT_H
#define TT_UTILS_BIT_H

/* //////////////////////////////////////////////////////////////////////////////////////
 * cl0
 */
static __inline tt_size_t tt_bits_cl0_u32_be_inline(tt_uint32_t x)
{
    tt_check_return_val(x, 32);

    tt_size_t n = 31;
    if (x & 0xffff0000) { n -= 16;  x >>= 16;   }
    if (x & 0xff00)     { n -= 8;   x >>= 8;    }
    if (x & 0xf0)       { n -= 4;   x >>= 4;    }
    if (x & 0xc)        { n -= 2;   x >>= 2;    }
    if (x & 0x2)        { n--;                  }

    return n;
}
static __inline tt_size_t tt_bits_cl0_u32_le_inline(tt_uint32_t x)
{
    tt_check_return_val(x, 32);

    tt_size_t n = 31;
    if (x & 0x0000ffff) { n -= 16;  } else x >>= 16;
    if (x & 0x00ff)     { n -= 8;   } else x >>= 8;
    if (x & 0x0f)       { n -= 4;   } else x >>= 4;
    if (x & 0x3)        { n -= 2;   } else x >>= 2;
    if (x & 0x1)        { n--;      }

    return n;
}
#if 0
static __inline tt_size_t tt_bits_cl0_u64_be_inline(tt_uint64_t x)
{
    tt_check_return_val(x, 64);

    tt_size_t n = tt_bits_cl0_u32_be((tt_uint32_t)(x >> 32));
    if (n == 32) n += tt_bits_cl0_u32_be((tt_uint32_t)x);

    return n;
}
static __inline tt_size_t tt_bits_cl0_u64_le_inline(tt_uint64_t x)
{
    tt_check_return_val(x, 64);

    tt_size_t n = tt_bits_cl0_u32_le((tt_uint32_t)x);
    if (n == 32) n += tt_bits_cl0_u32_le((tt_uint32_t)(x >> 32));

    return n;
}
#endif

#endif