/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_COMPILER_GCC_COMPILER_UTILS_H
#define TOS_COMPILER_GCC_COMPILER_UTILS_H

#include "../../utils/TestUtils.h"

#define PACKED_STRUCT  __attribute__((__packed__))
#define UNPACKED_STRUCT ((void) 0)

#define EXPORT_LIB extern "C" __attribute__((visibility("default")))

#if DEBUG
    #define UNREACHABLE() ASSERT_SIMPLE(false)
#else
    #define UNREACHABLE() __builtin_unreachable()
#endif

#endif