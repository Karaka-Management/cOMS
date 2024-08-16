/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_ENDIAN_H
#define TOS_UTILS_ENDIAN_H

#include "../stdlib/Types.h"

// Automatically perform endian swap if necessary
// If we are on little endian (e.g. Win32) we swap big endian data but not little endian
#if _WIN32 || __LITTLE_ENDIAN
    #define SWAP_ENDIAN_LITTLE(val) (val)
    #define SWAP_ENDIAN_BIG(val) endian_swap(val)
#else
    #define SWAP_ENDIAN_LITTLE(val) endian_swap(val)
    #define SWAP_ENDIAN_BIG(val) (val)
#endif

inline
bool is_little_endian()
{
    uint32 num = 1;
    return ((int32) (*(char *) & num)) == 1;
}

inline
uint16 endian_swap(const uint16* val)
{
    uint16 v = *val;
    return ((v << 8) | (v >> 8));
}

inline
int16 endian_swap(const int16* val)
{
    uint16 v = (uint16) (*val);
    return (int16) ((v << 8) | (v >> 8));
}

inline
uint32 endian_swap(const uint32* val)
{
    uint32 v = *val;
    return ((v << 24)
        | ((v & 0xFF00) << 8)
        | ((v >> 8) & 0xFF00)
        | (v >> 24));
}

inline
int32 endian_swap(const int32* val)
{
    uint32 v = (uint32) (*val);
    return (int32) ((v << 24)
        | ((v & 0xFF00) << 8)
        | ((v >> 8) & 0xFF00)
        | (v >> 24));
}

inline
uint64 endian_swap(const uint64* val)
{
    uint64 v = *val;
    return ((v << 56)
        | ((v & 0x000000000000FF00ULL) << 40)
        | ((v & 0x0000000000FF0000ULL) << 24)
        | ((v & 0x00000000FF000000ULL) << 8)
        | ((v & 0x000000FF00000000ULL) >> 8)
        | ((v & 0x0000FF0000000000ULL) >> 24)
        | ((v & 0x00FF000000000000ULL) >> 40)
        | (v >> 56));
}

inline
int64 endian_swap(const int64* val)
{
    uint64 v = (uint64) (*val);
    return (int64) ((v << 56)
        | ((v & 0x000000000000FF00ULL) << 40)
        | ((v & 0x0000000000FF0000ULL) << 24)
        | ((v & 0x00000000FF000000ULL) << 8)
        | ((v & 0x000000FF00000000ULL) >> 8)
        | ((v & 0x0000FF0000000000ULL) >> 24)
        | ((v & 0x00FF000000000000ULL) >> 40)
        | (v >> 56));
}

inline
float endian_swap(const float* val)
{
    uint32* ival = (uint32 *) val;
    return (float) endian_swap(ival);
}

inline
double endian_swap(const double* val)
{
    uint64* ival = (uint64 *) val;
    return (double) endian_swap(ival);
}

#endif