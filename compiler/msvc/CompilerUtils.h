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
#include <intrin.h>

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
#define compiler_prefetch_l1(mem) __prefetch((mem))
#define compiler_prefetch_l2(mem) __prefetch((mem))
#define compiler_prefetch_l3(mem) __prefetch((mem))

inline
int32 compiler_find_first_bit_r2l(uint64 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    return _BitScanForward64(&index, mask) ? index : -1;
}

inline
int32 compiler_find_first_bit_r2l(uint32 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    return _BitScanForward(&index, mask) ? index : -1;
}

inline
int32 compiler_find_first_bit_l2r(uint64 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    return _BitScanReverse64(&index, mask) ? index : -1;
}

inline
int32 compiler_find_first_bit_l2r(uint32 mask) {
    if (!mask) {
        return -1;
    }

    unsigned long index;
    return _BitScanReverse(&index, mask) ? index : -1;
}

inline
void compiler_cpuid(int32 cpuInfo[4], int32 function_id) {
    __cpuidex(cpuInfo, function_id, 0);
}

#endif