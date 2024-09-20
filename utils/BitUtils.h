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

#define IS_BIT_SET(num, pos) ((bool) ((num) & (1 << (pos))))
#define BIT_SET(num, pos) ((num) | ((uint32) 1 << (pos)))
#define BIT_UNSET(num, pos) ((num) & ~((uint32) 1 << (pos)))
#define BIT_FLIP(num, pos) ((num) ^ ((uint32) 1 << (pos)))
#define BIT_SET_TO(num, pos, x) ((num) & ~((uint32) 1 << (pos)) | ((uint32) (x) << (pos)))

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


inline
byte get_bits(byte data, int bits_to_read, int start_pos)
{
    byte mask = (1 << bits_to_read) - 1;
    return (data >> (8 - start_pos - bits_to_read)) & mask;
}

inline
uint64 get_bits(const byte* data, int bits_to_read, int start_pos)
{
    if (bits_to_read <= 0 || bits_to_read > sizeof(uint64)) {
        return 0;
    }

    int byte_index = start_pos / 8;
    int bit_offset = start_pos % 8;

    uint64_t mask = (1ULL << bits_to_read) - 1;
    uint64_t result = 0;

    int bits_read = 0;

    while (bits_read < bits_to_read) {
        int bits_in_current_byte = 8 - bit_offset;
        int bits_to_take = bits_to_read - bits_read;

        if (bits_to_take > bits_in_current_byte) {
            bits_to_take = bits_in_current_byte;
        }

        uint8_t current_byte = data[byte_index];
        current_byte >>= bit_offset;
        current_byte &= (1 << bits_to_take) - 1;

        result |= ((uint64_t)current_byte << bits_read);

        bits_read += bits_to_take;
        bit_offset = 0;
        byte_index++;
    }

    result &= mask;

    return result;
}

inline
uint32 reverse_bits(uint32 data, uint32 count)
{
    uint32 reversed = 0;
    for (uint32 i = 0; i <= (count / 2); ++i) {
        uint32 inv = count - i - 1;
        reversed |= ((data >> i) & 0x1) << inv;
        reversed |= ((data >> inv) & 0x1) << i;
    }

    return reversed;
}

#endif