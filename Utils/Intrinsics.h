/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_INTRINSICS_H
#define UTILS_INTRINSICS_H

#include <stdio.h>
#include <stdlib.h>

/*
MMX
Introduce eight 64 bit registers (MM0-MM7) and instructions to work with eight signed/unsigned bytes, four signed/unsigned words, two signed/unsigned dwords.

3DNow!
Add support for single precision floating point operand to MMX. Few operation supported, for example addition, subtraction, multiplication.

SSE
Introduce eight/sixteen 128 bit registers (XMM0-XMM7/15) and instruction to work with four single precision floating point operands. Add integer operations on MMX registers too. (The MMX-integer part of SSE is sometimes called MMXEXT, and was implemented on a few non-Intel CPUs without xmm registers and the floating point part of SSE.)

SSE2
Introduces instruction to work with 2 double precision floating point operands, and with packed byte/word/dword/qword integers in 128-bit xmm registers.

SSE3
Add a few varied instructions (mostly floating point), including a special kind of unaligned load (lddqu) that was better on Pentium 4, synchronization instruction, horizontal add/sub.

SSSE3
Again a varied set of instructions, mostly integer. The first shuffle that takes its control operand from a register instead of hard-coded (pshufb). More horizontal processing, shuffle, packing/unpacking, mul+add on bytes, and some specialized integer add/mul stuff.

SSE4 (SSE4.1, SSE4.2)
Add a lot of instructions: Filling in a lot of the gaps by providing min and max and other operations for all integer data types (especially 32-bit integer had been lacking), where previously integer min was only available for unsigned bytes and signed 16-bit. Also scaling, FP rounding, blending, linear algebra operation, text processing, comparisons. Also a non temporal load for reading video memory, or copying it back to main memory. (Previously only NT stores were available.)

AESNI
Add support for accelerating AES symmetric encryption/decryption.

AVX Add eight/sixteen 256 bit registers (YMM0-YMM7/15).
Support all previous floating point datatype. Three operand instructions.

FMA
Add Fused Multiply Add and correlated instructions.

AVX2
Add support for integer data types.

AVX512F
Add eight/thirty-two 512 bit registers (ZMM0-ZMM7/31) and eight 64-bit mask register (k0-k7). Promote most previous instruction to 512 bit wide. Optional parts of AVX512 add instruction for exponentials & reciprocals (AVX512ER), scatter/gather prefetching (AVX512PF), scatter conflict detection (AVX512CD), compress, expand.

IMCI (Intel Xeon Phi)
Early development of AVX512 for the first-gen Intel Xeon Phi (Knight's Corner) coprocessor.
*/

#ifndef __AVX__
    #define __AVX__ 0
#endif

#ifndef __AVX2__
    #define __AVX2__ 0
#endif

#ifndef __SSE__
    #define __SSE__ 0
#endif

#ifndef __SSE2__
    #define __SSE2__ 0
#endif

#ifndef __SSE2_MATH__
    #define __SSE2_MATH__ 0
#endif

#ifndef __SSE3__
    #define __SSE3__ 0
#endif

#ifndef __SSE4_1__
    #define __SSE4_1__ 0
#endif

#ifndef __SSE4_2__
    #define __SSE4_2__ 0
#endif

#ifndef __SSE_MATH__
    #define __SSE_MATH__ 0
#endif

#ifndef __SSSE3__
    #define __SSSE3__ 0
#endif

#endif