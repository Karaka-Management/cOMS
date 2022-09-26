/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_TEST_UTILS_H
#define UTILS_TEST_UTILS_H

#include <stdio.h>
#include "MathUtils.h"

#define ASSERT_EQUALS(a, b, t1, t2) ({\
        if ((a) == (b)) { \
            printf("."); \
        } else { \
            printf("[F]"); \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf((t1), (a)); printf(" != "); printf((t2), (b)); printf("\n"); \
            return 0; } \
    })

#define ASSERT_EQUALS_WITH_DELTA(a, b, delta, t1, t2) ({\
        if (oms_abs((a) - (b)) <= (delta)) { \
            printf("."); \
        } else { \
            printf("[F]"); \
            printf("\n\n%s - %i: ", __FILE__, __LINE__); \
            printf((t1), (a)); printf(" != "); printf((t2), (b)); printf("\n"); \
            return 0; } \
    })

#endif
