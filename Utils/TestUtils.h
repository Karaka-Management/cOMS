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

#include <stdio.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <time.h>
#endif

namespace Test {
    global_persist uint64 performance_count_frequency;
    struct TimingStat {
        uint64 oldFrame = 0;
        uint64 newFrame = 0;
        uint64 dc = 0;
        double dt = 0.0;
    };

    struct LogMessages {
        char *messages;
    };

    void update_timing_stat(TimingStat *stat)
    {
        #ifdef _WIN32
            LARGE_INTEGER counter;
            QueryPerformanceCounter(&counter);

            stat->newFrame = counter.QuadPart;
        #else
            struct timespec now;
            clock_gettime(CLOCK_MONOTONIC, &now);
            stat->newFrame = now.tv_sec + now.tv_nsec;
        #endif

        stat->dc = stat->newFrame - stat->oldFrame;
        stat->dt = (double) stat->dc / (double) performance_count_frequency;
        stat->oldFrame = stat->newFrame;
    }
}

#if DEBUG
    #define UPDATE_TIMING_STAT(stat) Test::update_timing_stat(stat);
#else
    #define UPDATE_TIMING_STAT(stat)
#endif

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

#define ASSERT_SIMPLE(a)                                 \
    if ((a) == false) {                                  \
        *(volatile int *)0 = 0;                          \
    }

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
