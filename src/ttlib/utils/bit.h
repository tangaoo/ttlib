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
 * includes
 */
#include "prefix.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// val=target variable, pos=bit number to act upon 0-n
#define tt_bit_set(val, pos)                 ((val) |= (1ULL<<(pos)))
#define tt_bit_clear(val, pos)               ((val) &= ~(1ULL<<(pos)))
#define tt_bit_flip(val, pos)                ((val) ^= (1ULL<<(pos)))
#define tt_bit_check(val, pos)               (!!((val) & (1ULL<<(pos))))  // '!!' to make sure this returns 0 or 1

// x=target variable, y=mask 
#define tt_bitmask_set(val, mask)            ((val) |= (mask))
#define tt_bitmask_clear(val, mask)          ((val) &= (~(mask)))
#define tt_bitmask_flip(val, mask)           ((val) ^= (mask))
#define tt_bitmask_check_all(val, mask)      (!(~(val) & (mask)))
#define tt_bitmask_check_any(val, mask)      ((val) & (mask))

/* //////////////////////////////////////////////////////////////////////////////////////
 * cl0
 */
static __tt_inline__ tt_size_t tt_bits_cl0_u32_be_inline(tt_uint32_t x)
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
static __tt_inline__ tt_size_t tt_bits_cl0_u32_le_inline(tt_uint32_t x)
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
static __tt_inline__ tt_size_t tt_bits_cl0_u64_be_inline(tt_uint64_t x)
{
    tt_check_return_val(x, 64);

    tt_size_t n = tt_bits_cl0_u32_be_inline((tt_uint32_t)(x >> 32));
    if (n == 32) n += tt_bits_cl0_u32_be_inline((tt_uint32_t)x);

    return n;
}
static __tt_inline__ tt_size_t tt_bits_cl0_u64_le_inline(tt_uint64_t x)
{
    tt_check_return_val(x, 64);

    tt_size_t n = tt_bits_cl0_u32_le_inline((tt_uint32_t)x);
    if (n == 32) n += tt_bits_cl0_u32_le_inline((tt_uint32_t)(x >> 32));

    return n;
}

#if (TT_CPU_BITSIZE == 32)
#   define tt_bits_fb0_le(x)   tt_bits_cl0_u32_le_inline(x)
#else
#   define tt_bits_fb0_le(x)   tt_bits_cl0_u64_le_inline(x)
#endif

#endif