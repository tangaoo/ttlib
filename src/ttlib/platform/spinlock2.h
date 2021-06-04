/*!The TT Library
 *
 * @Copyright (C) 2019-2021, TTLIB
 *
 * @file       spinlock2.c
 * @ingroup    platform
 * @author     tango
 * @date       2021-03-27 
 * @brief      spinlock2.c file
 */

#if 0

/* //////////////////////////////////////////////////////////////////////////////////////
 * includes
 */
#include "spinlock2.h"
#include "atomic.h"
#include <time.h>

/* //////////////////////////////////////////////////////////////////////////////////////
 * macros
 */
#if defined(__ARM64_ARCH_8__)
#   define tt_cpu_relax() __asm__ __volatile__("yield" ::: "memory")
#elif defined(__ARM_ARCH_7A__)
#   define tt_cpu_relax() __asm__ __volatile__("": : :"memory")
#elif defined(__ARM_ARCH_7__)
#   define tt_cpu_relax() __asm__ __volatile__("": : :"memory")
#else
#   define tt_cpu_relax() 
#endif

// the initial value
#define TT_SPINLOCK_INIT            TT_ATOMIC_FLAG_INIT

static __tt_inline_force__ tt_bool_t tt_spinlock_init(tt_spinlock_ref_t lock)
{
    // check
    tt_assert(lock);
    tt_atomic_flag_clear_explicit(lock, TT_ATOMIC_RELAXED);

    return tt_true;
}

static __tt_inline_force__ tt_void_t tt_spinlock_exit(tt_spinlock_ref_t lock)
{
    // check
    tt_assert(lock);
    tt_atomic_flag_clear_explicit(lock, TT_ATOMIC_RELAXED);
}

static __tt_inline_force__ tt_void_t tt_spinlock_entry(tt_spinlock_ref_t lock)
{
    // check
    tt_assert(lock);

    // cpu num
    tt_size_t cpu_n = sysconf(_SC_NPROCESSORS_CONF);

    while(1)
    {
        if(!tt_atomic_flag_test_noatomic(lock) && !tt_atomic_flag_test_and_set(lock))
            return;
        
        // if cpu > 1, busy loop and wait other thread release lock.
        if(n > 1)
        {
            tt_size_t i, n;
            for(n = 0; n < 2048; n <<= 1)
            {
                for(i = 1; i < n; i++)
                    tt_cpu_relax();

                if(!tt_atomic_flag_test_noatomic(lock) && !tt_atomic_flag_test_and_set(lock))
                    return;
            }
        }

        // quit cpu
        us_sleep(1);
    }
}

static __tt_inline_force__ tt_bool_t tt_spinlock_entry_try(tt_spinlock_ref_t lock)
{
    // check
    tt_assert(lock);

    return !tt_atomic_flag_test_and_set(lock);
}

static __tt_inline_force__ tt_void_t tt_spinlock_leave(tt_spinlock_ref_t lock)
{
    // check
    tt_assert(lock);

    tt_atomic_flag_clear(lock);
}

#endif