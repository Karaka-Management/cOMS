/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef STDLIB_TYPES_H
#define STDLIB_TYPES_H

#include <float.h>
#include <stdint.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float f32;
typedef double f64;

typedef unsigned char byte;

#define internal static // only allows local "file" access
#define local_persist static
#define global_persist static

#endif
