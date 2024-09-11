/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_BIT_H
#define TOS_UTILS_BIT_H

#include <intrin.h>
#include "../stdlib/Types.h"

inline
uint32 bytes_merge(byte b0, byte b1, byte b2, byte b3) {
    uint32 result = 0;

    result |= ((uint32) b0 << 24);
    result |= ((uint32) b1 << 16);
    result |= ((uint32) b2 << 8);
    result |= (uint32) b3;

    return result;
}

inline
uint64 bytes_merge(
    byte b0, byte b1, byte b2, byte b3,
    byte b4, byte b5, byte b6, byte b7
) {
    uint64 result = 0;

    result |= ((uint64) b0 << 56);
    result |= ((uint64) b1 << 48);
    result |= ((uint64) b2 << 40);
    result |= ((uint64) b3 << 32);
    result |= ((uint64) b4 << 24);
    result |= ((uint64) b5 << 16);
    result |= ((uint64) b6 << 8);
    result |= (uint64) b7;

    return result;
}

static
inline int find_first_set_bit(int value) {
    if (value == 0) {
        return 0;
    }

    #if __GNUC__ || __clang__
        return __builtin_ffs(value);
    #elif _MSC_VER
        unsigned long index; // For _BitScanForward, an unsigned long is expected
        if (_BitScanForward(&index, value)) {
            return (int)index + 1; // Convert to 1-based index
        } else {
            return 0; // No set bit found
        }
    #else
        int index = 1; // Start at 1 for 1-based index
        while (value) {
            if (value & 1) {
                return index;
            }
            value >>= 1; // Shift right to check the next bit
            index++;
        }
        return 0; // No set bit found
    #endif
}

#endif