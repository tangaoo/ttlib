/*!The TT Library
 *
 * @Copyright (C) 2019-2020, TTLIB
 *
 * @file       static_large_allocator.c
 * @ingroup    memory
 * @author     tango
 * @date       2020-12-25 
 * @brief      static_large_allocator.c file
 */

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "static_large_allocator.h"

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */

// get base of head_data
#define tt_static_large_data_allocator_base(head_data)  (&(((tt_pool_data_head_t*)((tt_static_large_data_head_t*)(data_head) + 1))[-1]))  //TODO &

// get next head_data
#define tt_static_large_head_next(head_data)    (tt_static_large_data_head_t*)(head_data) + ((tt_static_large_data_head_t*)(head_data))->space

/* //////////////////////////////////////////////////////////////////////////////////////
 * types
 */

// the static large data head type
typedef __tt_aligned__(TT_CPU_BITBYTE) struct __tt_static_large_data_head_t 
{
    // the data space
    tt_size_t   space : 31;

    // the data be free?
    tt_size_t   bfree : 1 ;

    // the data head base
    tt_byte_t   base[sizeof(tt_pool_data_head_t)];

}__tt_aligned__(TT_CPU_BITBYTE) tt_static_large_data_head_t, *tt_static_large_data_head_ref_t;

// the static large data pred type
typedef struct __tt_static_large_data_pred_t
{
    // the static large data head
    tt_static_large_data_head_t*  data_head;

}tt_static_large_data_pred_t, *tt_static_large_data_pred_ref_t;

/*! the static large allocator type
 *
 * <pre>
 *
 * .e.g page_size == 4KB
 *
 *       +--------------------------------------------------------------------------+
 *       |                                     data                                 |
 *       +--------------------------------------------------------------------------+
 *                                              |
 *       +--------------------------------------------------------------------------+
 *       | head | 4KB | 16KB | 8KB | 128KB | ... | 32KB |       ...       |  4KB*N  |
 *       +--------------------------------------------------------------------------+
 *                       |                       |               |
 *                       |                       `---------------`
 *                       |                        merge free space when alloc or free
 *                       |
 *       +------------------------------------------+
 *       | tb_static_large_data_head_t | data space |
 *       +------------------------------------------+
 *                       |
 *                       |
 *       +----------------------+
 *       | space | bfree | base | 
 *       +----------------------+
 * 
 * 
 *       +--------------------------------------+
 * pred: | >0KB :      4KB       | > 0*page     | 1
 *       |-----------------------|--------------+
 *       | >4KB :      8KB       | > 1*page     | 2
 *       |-----------------------|--------------+
 *       | >8KB :    12-16KB     | > 2*page     | 3-4
 *       |-----------------------|--------------+
 *       | >16KB :   20-32KB     | > 4*page     | 5-8
 *       |-----------------------|--------------+
 *       | >32KB :   36-64KB     | > 8*page     | 9-16
 *       |-----------------------|--------------+
 *       | >64KB :   68-128KB    | > 16*page    | 17-32
 *       |-----------------------|--------------+
 *       | >128KB :  132-256KB   | > 32*page    | 33-64
 *       |-----------------------|--------------+
 *       | >256KB :  260-512KB   | > 64*page    | 65 - 128
 *       |-----------------------|--------------+
 *       | >512KB :  516-1024KB  | > 128*page   | 129 - 256
 *       |-----------------------|--------------+
 *       | >1024KB : 1028-...KB  | > 256*page   | 257 - ..
 *       +--------------------------------------+
 *
 * </pre>
 */

// the static large allocator type
typedef __tt_aligned__(TT_CPU_BITBYTE) struct __tt_static_large_allocator_t
{
    // base
    tt_allocator_t                 base;

    // the page size
    tt_size_t                      page_size;

    // the data total size
    tt_size_t                      data_size;

    // the data head
    tt_static_large_data_head_t*   data_head; 

    // the data tail
    tt_static_large_data_head_t*   data_tail; 

    // the data pred
    tt_static_large_data_pred_t    pred[10];

}__tt_aligned__(TT_CPU_BITBYTE) tt_static_large_allocator_t, *tt_static_large_allocator_ref_t;
