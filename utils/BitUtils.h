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

//#include <intrin.h>
#include "../stdlib/Types.h"

// @todo Replace many of these functions with intrinsic functions
//      This file can remain but the callers should get adjusted
//      Obviously we would have to check at runtime if ABM is supported

// Left to right
#define IS_BIT_SET_L2R(num, pos, bits) ((bool) ((num) & (1 << ((bits - 1) - (pos)))))
#define BIT_SET_L2R(num, pos, bits) ((num) | ((uint32) 1 << ((bits - 1) - (pos))))
#define BIT_UNSET_L2R(num, pos, bits) ((num) & ~((uint32) 1 << ((bits - 1) - (pos))))
#define BIT_FLIP_L2R(num, pos, bits) ((num) ^ ((uint32) 1 << ((bits - 1) - (pos))))
#define BIT_SET_TO_L2R(num, pos, x, bits) ((num) & ~((uint32) 1 << ((bits - 1) - (pos))) | ((uint32) (x) << ((bits - 1) - (pos))))
#define BITS_GET_8_L2R(num, pos, to_read) (((num) >> (8 - (pos) - (to_read))) & ((1U << (to_read)) - 1))
#define BITS_GET_16_L2R(num, pos, to_read) (((num) >> (16 - (pos) - (to_read))) & ((1U << (to_read)) - 1))
#define BITS_GET_32_L2R(num, pos, to_read) (((num) >> (32 - (pos) - (to_read))) & ((1U << (to_read)) - 1))
#define BITS_GET_64_L2R(num, pos, to_read) (((num) >> (64 - (pos) - (to_read))) & ((1ULL << (to_read)) - 1))

#define BYTES_MERGE_2_L2R(num) (((num)[0] << 8) | (num)[1])
#define BYTES_MERGE_4_L2R(num) (((num)[0] << 24) | ((num)[1] << 16) | ((num)[2] << 8) | (num)[3])
#define BYTES_MERGE_8_L2R(num) (((uint64_t)(num)[0] << 56) | ((uint64_t)(num)[1] << 48) | ((uint64_t)(num)[2] << 40) | ((uint64_t)(num)[3] << 32) | ((uint64_t)(num)[4] << 24) | ((uint64_t)(num)[5] << 16) | ((uint64_t)(num)[6] << 8)  | ((uint64_t)(num)[7]))

// Right to left
#define IS_BIT_SET_R2L(num, pos) ((bool) ((num) & (1 << (pos))))
#define BIT_SET_R2L(num, pos) ((num) | ((uint32) 1 << (pos)))
#define BIT_UNSET_R2L(num, pos) ((num) & ~((uint32) 1 << (pos)))
#define BIT_FLIP_R2L(num, pos) ((num) ^ ((uint32) 1 << (pos)))
#define BIT_SET_TO_R2L(num, pos, x) ((num) & ~((uint32) 1 << (pos)) | ((uint32) (x) << (pos)))
// @performance Try to use this version over the L2R version for performance reasons
#define BITS_GET_8_R2L(num, pos, to_read) (((num) >> (pos)) & ((1U << (to_read)) - 1))
#define BITS_GET_16_R2L(num, pos, to_read) (((num) >> (pos)) & ((1U << (to_read)) - 1))
#define BITS_GET_32_R2L(num, pos, to_read) (((num) >> (pos)) & ((1U << (to_read)) - 1))
#define BITS_GET_64_R2L(num, pos, to_read) (((num) >> (pos)) & ((1ULL << (to_read)) - 1))

#define BYTES_MERGE_2_R2L(num) (((num)[1] << 8) | (num)[0])
#define BYTES_MERGE_4_R2L(num) (((num)[3] << 24) | ((num)[2] << 16) | ((num)[1] << 8) | (num)[0])
#define BYTES_MERGE_8_R2L(num) (((uint64_t)(num)[7] << 56) | ((uint64_t)(num)[6] << 48) | ((uint64_t)(num)[5] << 40) | ((uint64_t)(num)[4] << 32) | ((uint64_t)(num)[3] << 24) | ((uint64_t)(num)[2] << 16) | ((uint64_t)(num)[1] << 8)  | ((uint64_t)(num)[0]))

struct BitWalk {
    byte* pos;
    uint32 bit_pos;
};

inline
void bits_walk(BitWalk* stream, uint32 bits_to_walk)
{
    stream->bit_pos += bits_to_walk;
    stream->pos += stream->bit_pos / 8;
    stream->bit_pos %= 8;
}

inline
void bits_flush(BitWalk* stream)
{
    if (stream->bit_pos > 0) {
        stream->bit_pos = 0;
        ++stream->pos;
    }
}

// inline
// uint8 bits_consume_8(BitWalk* stream, uint32 bits_to_consume)
// {
//     uint8 result;

//     uint32 remaining = 8 - stream->bit_pos;
//     uint32 range_1 = bits_to_consume >= remaining
//         ? remaining
//         : bits_to_consume;

//     result = (*stream->pos >> (remaining - range_1)) & ((1 << range_1) - 1);
//     stream->bit_pos += range_1;

//     if (bits_to_consume < remaining) {
//         return result;
//     }

//     ++stream->pos;
//     stream->bit_pos = 0;
//     bits_to_consume -= range_1;

//     /*
//     uint32 full_bytes = bits_to_consume / 8;
//     if (full_bytes > 0) {
//         for (int i = 0; i < full_bytes; ++i) {
//             result = (result << 8) | *stream->pos;

//             ++stream->pos;
//         }
//     }
//     */

//     if (bits_to_consume == 0) {
//         return result;
//     }

//     stream->bit_pos += bits_to_consume;

//     return (result << bits_to_consume) | ((*stream->pos >> (8 - bits_to_consume)) & ((1 << bits_to_consume) - 1));
// }

// inline
// uint16 bits_consume_16(BitWalk* stream, uint32 bits_to_consume)
// {
//     uint16 result;

//     uint32 remaining = 8 - stream->bit_pos;
//     uint32 range_1 = bits_to_consume >= remaining
//         ? remaining
//         : bits_to_consume;

//     result = (*stream->pos >> (remaining - range_1)) & ((1 << range_1) - 1);
//     stream->bit_pos += range_1;

//     if (bits_to_consume < remaining) {
//         return result;
//     }

//     ++stream->pos;
//     stream->bit_pos = 0;
//     bits_to_consume -= range_1;

//     uint32 full_bytes = bits_to_consume / 8;
//     if (full_bytes > 0) {
//         for (int i = 0; i < full_bytes; ++i) {
//             result = (result << 8) | *stream->pos;

//             ++stream->pos;
//         }
//     }

//     uint32 range_2 = bits_to_consume - full_bytes * 8;
//     if (range_2 == 0) {
//         return result;
//     }

//     stream->bit_pos += range_2;

//     return (result << range_2) | ((*stream->pos >> (8 - range_2)) & ((1 << range_2) - 1));
// }

// inline
// uint32 bits_consume_32(BitWalk* stream, uint32 bits_to_consume)
// {
//     uint32 result;

//     uint32 remaining = 8 - stream->bit_pos;
//     uint32 range_1 = bits_to_consume >= remaining
//         ? remaining
//         : bits_to_consume;

//     result = (*stream->pos >> (remaining - range_1)) & ((1 << range_1) - 1);
//     stream->bit_pos += range_1;

//     if (bits_to_consume < remaining) {
//         return result;
//     }

//     ++stream->pos;
//     stream->bit_pos = 0;
//     bits_to_consume -= range_1;

//     uint32 full_bytes = bits_to_consume / 8;
//     if (full_bytes > 0) {
//         for (int i = 0; i < full_bytes; ++i) {
//             result = (result << 8) | *stream->pos;

//             ++stream->pos;
//         }
//     }

//     uint32 range_2 = bits_to_consume - full_bytes * 8;
//     if (range_2 == 0) {
//         return result;
//     }

//     stream->bit_pos += range_2;

//     return (result << range_2) | ((*stream->pos >> (8 - range_2)) & ((1 << range_2) - 1));
// }

// inline
// uint64 bits_consume_64(BitWalk* stream, uint32 bits_to_consume)
// {
//     uint64 result;

//     uint32 remaining = 8 - stream->bit_pos;
//     uint32 range_1 = bits_to_consume >= remaining
//         ? remaining
//         : bits_to_consume;

//     result = (*stream->pos >> (remaining - range_1)) & ((1 << range_1) - 1);
//     stream->bit_pos += range_1;

//     if (bits_to_consume < remaining) {
//         return result;
//     }

//     ++stream->pos;
//     stream->bit_pos = 0;
//     bits_to_consume -= range_1;

//     uint32 full_bytes = bits_to_consume / 8;
//     if (full_bytes > 0) {
//         for (int i = 0; i < full_bytes; ++i) {
//             result = (result << 8) | *stream->pos;

//             ++stream->pos;
//         }
//     }

//     uint32 range_2 = bits_to_consume - full_bytes * 8;
//     if (range_2 == 0) {
//         return result;
//     }

//     stream->bit_pos += range_2;

//     return (result << range_2) | ((*stream->pos >> (8 - range_2)) & ((1 << range_2) - 1));
// }

// uint8 bits_peek_8(BitWalk* stream, uint32 bits_to_consume) {
//     byte* pos = stream->pos;
//     byte bit_pos = stream->bit_pos;

//     uint8 bits = bits_consume_8(stream, bits_to_consume);

//     stream->pos = pos;
//     stream->bit_pos = bit_pos;

//     return bits;
// }

// uint16 bits_peek_16(BitWalk* stream, uint32 bits_to_consume) {
//     byte* pos = stream->pos;
//     byte bit_pos = stream->bit_pos;

//     uint16 bits = bits_consume_16(stream, bits_to_consume);

//     stream->pos = pos;
//     stream->bit_pos = bit_pos;

//     return bits;
// }

// uint32 bits_peek_32(BitWalk* stream, uint32 bits_to_consume) {
//     byte* pos = stream->pos;
//     byte bit_pos = stream->bit_pos;

//     uint32 bits = bits_consume_32(stream, bits_to_consume);

//     stream->pos = pos;
//     stream->bit_pos = bit_pos;

//     return bits;
// }

// uint64 bits_peek_64(BitWalk* stream, uint32 bits_to_consume) {
//     byte* pos = stream->pos;
//     byte bit_pos = stream->bit_pos;

//     uint64 bits = bits_consume_64(stream, bits_to_consume);

//     stream->pos = pos;
//     stream->bit_pos = bit_pos;

//     return bits;
// }

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
inline int32 find_first_set_bit(int32 value) {
    if (value == 0) {
        return 0;
    }

    #if __GNUC__ || __clang__
        return __builtin_ffs(value);
    #elif _MSC_VER
        unsigned long index; // For _BitScanForward, an unsigned long is expected
        if (_BitScanForward(&index, value)) {
            return (int32) index + 1; // Convert to 1-based index
        } else {
            return 0; // No set bit found
        }
    #else
        int32 index = 1; // Start at 1 for 1-based index
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
uint32 bits_reverse(uint32 data, uint32 count)
{
    uint32 reversed = 0;
    for (uint32 i = 0; i <= (count / 2); ++i) {
        uint32 inv = count - i - 1;
        reversed |= ((data >> i) & 0x1) << inv;
        reversed |= ((data >> inv) & 0x1) << i;
    }

    return reversed;
}

const int32 BIT_COUNT_LOOKUP_TABLE[256] = {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};

int32 bits_count(uint64 data) {
    return BIT_COUNT_LOOKUP_TABLE[data & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 8) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 16) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 24) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 32) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 40) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 48) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 56) & 0xFF];
}

int32 bits_count(uint32 data) {
    return BIT_COUNT_LOOKUP_TABLE[data & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 8) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 16) & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 24) & 0xFF];
}

int32 bits_count(uint16 data) {
    return BIT_COUNT_LOOKUP_TABLE[data & 0xFF]
        + BIT_COUNT_LOOKUP_TABLE[(data >> 8) & 0xFF];
}

int32 bits_count(uint8 data) {
    return BIT_COUNT_LOOKUP_TABLE[data];
}

#endif