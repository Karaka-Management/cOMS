/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_TEST_UTILS_H
#define UTILS_TEST_UTILS_H

#include "MathUtils.h"
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
        if (oms_abs((a) - (b)) <= (delta)) {             \
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

#endif
