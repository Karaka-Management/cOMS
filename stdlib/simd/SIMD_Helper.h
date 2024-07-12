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

#ifdef _MSC_VER
    #include <intrin.h>
#endif

inline
bool is_sse_supported()
{
    #ifdef _MSC_VER
        int cpuInfo[4] = {-1};
        __cpuid(cpuInfo, 1); // CPUID function 1

        // Check the SSE feature bit in EDX
        return (cpuInfo[3] >> 25) & 1;
    #else
        uint32_t eax, ebx, ecx, edx;

        eax = 1; // CPUID function 1

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax));

        // Check the AVX feature bit in ECX
        return (ecx >> 28) & 1;
    #endif
}

inline
bool is_avx256_supported()
{
    #ifdef _MSC_VER
        int cpuInfo[4] = {-1};
        __cpuid(cpuInfo, 1); // CPUID function 1

        // Check the AVX feature bit in ECX
        if ((cpuInfo[2] >> 28) & 1) {
            __cpuid(cpuInfo, 7); // CPUID function 7, sub-function 0
            // Check the AVX2 feature bit in EBX
            return (cpuInfo[1] >> 5) & 1;
        }

        return false;
    #else
        uint32_t eax, ebx, ecx, edx;

        eax = 7; // CPUID function 7
        ecx = 0; // Sub-function 0

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax), "c"(ecx));

        // Check the AVX-256 (AVX2) feature bit in EBX
        return (ebx >> 5) & 1;
    #endif
}

inline
bool is_avx512_supported()
{
    #ifdef _MSC_VER
        int cpuInfo[4] = {-1};
        __cpuid(cpuInfo, 1); // CPUID function 1

        // Check the AVX feature bit in ECX
        if ((cpuInfo[2] >> 28) & 1) {
            __cpuid(cpuInfo, 7); // CPUID function 7, sub-function 0
            // Check the AVX-512 feature bit in EBX
            return (cpuInfo[1] >> 16) & 1;
        }

        return false;
    #else
        uint32_t eax, ebx, ecx, edx;

        eax = 7; // CPUID function 7
        ecx = 0; // Sub-function 0

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax), "c"(ecx));

        // Check the AVX-512 feature bit in EBX
        return (ebx >> 16) & 1;
    #endif
}

#endif