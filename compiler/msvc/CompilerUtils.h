/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_COMPILER_MSVC_COMPILER_UTILS_H
#define TOS_COMPILER_MSVC_COMPILER_UTILS_H

#include "../../utils/TestUtils.h"
#include <basetsd.h>

#define PACKED_STRUCT  __pragma(pack(push, 1))
#define UNPACKED_STRUCT __pragma(pack(pop))

#define EXPORT_LIB extern "C" __declspec(dllexport)

typedef SSIZE_T ssize_t;

#if DEBUG
    #define UNREACHABLE() ASSERT_SIMPLE(false) __assume(0)
#else
    #define UNREACHABLE() __assume(0)
#endif

#define FORCE_INLINE __forceinline

#define compiler_popcount_32(data) __popcnt((data))
#define compiler_popcount_64(data) __popcnt64((data))

#define compiler_prefetch(mem) __prefetch((mem))

inline
int32 compiler_find_first_bit_r2l(uint64 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    _BitScanForward64(&index, mask);

    return index;
}

inline
int32 compiler_find_first_bit_r2l(uint32 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    _BitScanForward(&index, mask);

    return index;
}

inline
int32 compiler_find_first_bit_l2r(uint64 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    _BitScanReverse64(&index, mask);

    return index;
}

inline
int32 compiler_find_first_bit_l2r(uint32 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    _BitScanReverse(&index, mask);

    return index;
}

#endif