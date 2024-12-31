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

#include <stdio.h>

#define ASSERT_EQUALS(a, b, t1, t2)                      \
    ({                                                   \
        if ((a) == (b)) {                                \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf((t1), (a));                           \
            printf(" != ");                              \
            printf((t2), (b));                           \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#define ASSERT_NOT_EQUALS(a, b, t1, t2)                  \
    ({                                                   \
        if ((a) != (b)) {                                \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf((t1), (a));                           \
            printf(" == ");                              \
            printf((t2), (b));                           \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#define ASSERT_EQUALS_WITH_DELTA(a, b, delta, t1, t2)    \
    ({                                                   \
        if (OMS_ABS((a) - (b)) <= (delta)) {             \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf((t1), (a));                           \
            printf(" != ");                              \
            printf((t2), (b));                           \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#define ASSERT_CONTAINS(a, b)                            \
    ({                                                   \
        if (strstr((a), (b)) != NULL) {                  \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf("%s", (a));                           \
            printf(" !contains ");                       \
            printf("%s", (b));                           \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#if DEBUG
    #define ASSERT_SIMPLE(a)                             \
        if (!(a)) {                                      \
            *(volatile int *) 0 = 0;                     \
        }

    #define ASSERT_SIMPLE_CONST(a)                       \
        if constexpr (!(a)) {                            \
            *(volatile int *) 0 = 0;                     \
        }
#else
    #define ASSERT_SIMPLE(a) ((void) 0)
    #define ASSERT_SIMPLE_CONST(a) ((void) 0)
#endif

#define ASSERT_TRUE(a)                                   \
    ({                                                   \
        if ((a) == true) {                               \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf("%d", (a));                           \
            printf(" != ");                              \
            printf("1");                                 \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#define ASSERT_FALSE(a)                                  \
    ({                                                   \
        if ((a) == false) {                              \
            printf(".");                                 \
        } else {                                         \
            printf("\033[31m[F]\033[0m");                \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf("%d", (a));                           \
            printf(" != ");                              \
            printf("1");                                 \
            printf("\n");                                \
            return 0;                                    \
        }                                                \
    })

#define ASSERT_PERFORMANCE_START(time_start)             \
({                                                       \
    time_start = __rdtsc();                              \
})

#define ASSERT_PERFORMANCE_END(time_start, max_duration)        \
({                                                              \
    ASSERT_TRUE(__rdtsc() - (time_start) <= (max_duration));    \
})

#endif
