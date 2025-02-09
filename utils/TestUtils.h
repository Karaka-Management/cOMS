/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_TEST_UTILS_H
#define TOS_UTILS_TEST_UTILS_H

#include "../architecture/Intrinsics.h"

#if DEBUG
    #define ASSERT_SIMPLE(a)                     \
        if (!(a))                                \
        {                                        \
            *(volatile int *)0 = 0;              \
        }

    #define ASSERT_SIMPLE_CONST(a)               \
        if constexpr (!(a))                      \
        {                                        \
            *(volatile int *)0 = 0;              \
        }

    #define ASSERT_PERFORMANCE_START(time_start)     \
        ({                                           \
            time_start = intrin_timestamp_counter(); \
        })

    #define ASSERT_PERFORMANCE_END(time_start, max_duration)                            \
        ({                                                                              \
            ASSERT_SIMPLE(intrin_timestamp_counter() - (time_start) <= (max_duration)); \
        })
#else
    #define ASSERT_SIMPLE(a) ((void)0)
    #define ASSERT_SIMPLE_CONST(a) ((void)0)
    #define ASSERT_PERFORMANCE_START(time_start) ((void)0)
    #define ASSERT_PERFORMANCE_END(time_start, max_duration) ((void)0)
#endif

#endif
