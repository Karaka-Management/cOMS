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
#include "../stdlib/Types.h"

struct FileBody {
    uint64 size; // doesn't include null termination (same as strlen)
    byte* content;
};

// @question Do we want to make the size comparison a step variable?
inline
bool is_equal_aligned(const byte* region1, const byte* region2, uint64 size)
{
    while (size > 4) {
        if (*(const int32 *) region1 != *(const int32 *) region2) {
            return false;
        }

        region1 += 4;
        region2 += 4;
        size -= 4;
    }

    for (; size > 0; --size) {
        if (region1 != region2) {
            return false;
        }

        ++region1;
        ++region2;
    }

    return true;
}

// @question Do we want to make the size comparison a step variable?
bool is_empty(const byte* region, uint64 size)
{
    while (size > 4) {
        if (*(const int32 *) region != 0) {
            return false;
        }

        region += 4;
        size -= 4;
    }

    for (; size > 0; --size) {
        if (region != 0) {
            return false;
        }

        ++region;
    }

    return true;
}

#endif