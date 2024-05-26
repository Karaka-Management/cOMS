/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef STDLIB_SIMD_HELPER_H
#define STDLIB_SIMD_HELPER_H

#include <immintrin.h>
#include <stdint.h>
#include <xmmintrin.h>

namespace Stdlib::SIMD
{
    bool is_sse_supported()
    {
        uint32_t eax, ebx, ecx, edx;

        eax = 1; // CPUID function 1

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax));

        // Check the AVX feature bit in ECX
        return (ecx >> 28) & 1;
    }

    bool is_avx256_supported()
    {
        uint32_t eax, ebx, ecx, edx;

        eax = 7; // CPUID function 7
        ecx = 0; // Sub-function 0

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax), "c"(ecx));

        // Check the AVX-256 (AVX2) feature bit in EBX
        return (ebx >> 5) & 1;
    }

    bool is_avx512_supported()
    {
        uint32_t eax, ebx, ecx, edx;

        eax = 7; // CPUID function 7
        ecx = 0; // Sub-function 0

        __asm__ __volatile__("cpuid;" : "=a"(eax), "=b"(ebx), "=c"(ecx), "=d"(edx) : "a"(eax), "c"(ecx));

        // Check the AVX-512 feature bit in EBX
        return (ebx >> 16) & 1;
    }
} // namespace Stdlib::SIMD

#endif