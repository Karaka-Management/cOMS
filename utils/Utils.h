/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_H
#define TOS_UTILS_H

#include <stdlib.h>
#include <time.h>

#include "../stdlib/Types.h"

struct FileBody {
    uint64 size = 0; // doesn't include null termination (same as strlen)
    byte* content;
};

global_persist uint32 fast_seed;
#define FAST_RAND_MAX 32767

inline
uint32 fast_rand1(void) {
    fast_seed = (214013 * fast_seed + 2531011);

    return (fast_seed >> 16) & 0x7FFF;
}

uint32 fast_rand2(uint32* state) {
    uint32 x = *state;

    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;

    *state = x;

    return x;
}

inline
f32 fast_rand_percentage(void) {
    return (f32) fast_rand1() / (f32) FAST_RAND_MAX;
}

/**
 * Picks n random elements from end and stores them in begin.
 */
inline
void random_unique(int32* array, int32 size) {
    for (int32 i = size - 1; i > 0; --i) {
        int32 j = rand() % (i + 1);

        int32 temp = array[i];
        array[i] = array[j];
        array[j] = temp;
    }
}

/**
 * Gets random index based value probability
 */
int random_weighted_index(const int32* arr, int32 array_count)
{
    uint32 prob_sum = 0;
    for (int32 i = 0; i < array_count; ++i) {
        prob_sum += arr[i];
    }

    uint32 random_prob = rand() % (prob_sum + 1);
    uint32 current_rarity = 0;
    int32 item_rarity = array_count - 1;
    for (int32 i = 0; i < array_count - 1; ++i) {
        current_rarity += arr[i];

        if (current_rarity < random_prob) {
            item_rarity = i;
            break;
        }
    }

    return item_rarity;
}

#endif