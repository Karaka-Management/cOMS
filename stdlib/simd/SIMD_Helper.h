/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_SIMD_HELPER_H
#define TOS_STDLIB_SIMD_HELPER_H

#include <stdint.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include "../Types.h"

#ifdef _MSC_VER
    #include <intrin.h>
#endif

enum SIMDVersion {
    SIMD_VERSION_NONE,
    SIMD_VERSION_128,
    SIMD_VERSION_256,
    SIMD_VERSION_512,
};

// @todo implement for arm?

inline int32 max_sse_supported()
{
    #ifdef _MSC_VER
        int32 cpuInfo[4] = {-1};
        __cpuid(cpuInfo, 1); // CPUID function 1

        uint32 ecx = cpuInfo[2];
        uint32 edx = cpuInfo[3];
    #else
        uint32 eax, ebx, ecx, edx;

        eax = 1; // CPUID function 1
        __asm__ __volatile__("cpuid;"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(eax));
    #endif

    bool sse42_supported = (ecx >> 20) & 1;
    if (sse42_supported) {
        return 42;
    }

    bool sse41_supported = (ecx >> 19) & 1;
    if (sse41_supported) {
        return 41;
    }

    bool sse3_supported = (ecx >> 0) & 1;
    if (sse3_supported) {
        return 3;
    }

    bool sse2_supported = (edx >> 26) & 1;
    if (sse2_supported) {
        return 2;
    }

    return 0;
}

inline
int max_avx256_supported()
{
    int32 max_version = 0;

    #ifdef _MSC_VER
        int32 cpuInfo[4];
        __cpuid(cpuInfo, 1);

        if ((cpuInfo[2] >> 28) & 1) {
            __cpuid(cpuInfo, 7); // Query extended features

            if ((cpuInfo[1] >> 5) & 1) {
                max_version = 2;
            }
        }
    #else
        uint32 eax, ebx, ecx, edx;

        __asm__ __volatile__("cpuid"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(1));
        if ((ecx >> 28) & 1) {
            eax = 7;
            ecx = 0;
            __asm__ __volatile__("cpuid"
                                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                                 : "a"(eax), "c"(ecx));

            if ((ebx >> 5) & 1) {
                max_version = 2;
            }
        }
    #endif

    return max_version;
}

inline
int max_avx512_supported()
{
    #ifdef _MSC_VER
        int32 cpuInfo[4];
        __cpuid(cpuInfo, 1);
        int32 ebx = 0;

        if ((cpuInfo[2] >> 28) & 1) {
            __cpuid(cpuInfo, 7);

            ebx = cpuInfo[1];
        }
    #else
        uint32 eax, ebx, ecx, edx;

        __asm__ __volatile__("cpuid"
                             : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                             : "a"(1));
        if ((ecx >> 28) & 1) {
            eax = 7;
            ecx = 0;
            __asm__ __volatile__("cpuid"
                                 : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
                                 : "a"(eax), "c"(ecx));
        }
    #endif

    if ((ebx >> 16) & 1) {
        return 1; // AVX-512F
    }

    if ((ebx >> 17) & 1) {
        return 2; // AVX-512DQ
    }

    if ((ebx >> 21) & 1) {
        return 3; // AVX-512IFMA
    }

    if ((ebx >> 26) & 1) {
        return 4; // AVX-512PF
    }

    if ((ebx >> 27) & 1) {
        return 5; // AVX-512ER
    }

    if ((ebx >> 28) & 1) {
        return 6; // AVX-512CD
    }

    if ((ebx >> 30) & 1) {
        return 7; // AVX-512BW
    }

    if ((ebx >> 31) & 1) {
        return 8; // AVX-512VL
    }

    return 0;
}

const char AVX512_VERSIONS[8][12] = {
    "AVX-512F",
    "AVX-512DQ",
    "AVX-512IFMA",
    "AVX-512PF",
    "AVX-512ER",
    "AVX-512CD",
    "AVX-512BW",
    "AVX-512VL"
};

bool supports_abm() {
    bool popcnt_supported;
    bool lzcnt_supported;

    #ifdef _MSC_VER
        int cpuInfo[4];
        __cpuid(cpuInfo, 0x80000001);

        popcnt_supported = (cpuInfo[2] & (1 << 5)) != 0;
        lzcnt_supported = (cpuInfo[1] & (1 << 5)) != 0;
    #else
        uint32 eax, ebx, ecx, edx;
        eax = 0x80000001;

        __asm__ __volatile__ (
            "cpuid"
            : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx)
            : "a"(eax)
        );

        // Check if the ABM (POPCNT and LZCNT) bits are set
        popcnt_supported = (ecx & (1 << 5)) != 0;
        lzcnt_supported = (ebx & (1 << 5)) != 0;
    #endif


    return popcnt_supported && lzcnt_supported;
}

#endif