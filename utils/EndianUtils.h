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
void endian_swap(uint16 *val)
{
    uint16 v = *val;
    *val = ((v << 8) | (v >> 8));
}

inline
void endian_swap(int16 *val)
{
    uint16 v = (uint16) (*val);
    *val = (int16) ((v << 8) | (v >> 8));
}

inline
void endian_swap(uint32 *val)
{
    uint32 v = *val;
    *val = ((v << 24)
        | ((v & 0xFF00) << 8)
        | ((v >> 8) & 0xFF00)
        | (v >> 24));
}

inline
void endian_swap(int32 *val)
{
    uint32 v = (uint32) (*val);
    *val = (int32) ((v << 24)
        | ((v & 0xFF00) << 8)
        | ((v >> 8) & 0xFF00)
        | (v >> 24));
}

inline
void endian_swap(uint64 *val)
{
    uint64 v = *val;
    *val = ((v << 56)
        | ((v & 0x000000000000FF00ULL) << 40)
        | ((v & 0x0000000000FF0000ULL) << 24)
        | ((v & 0x00000000FF000000ULL) << 8)
        | ((v & 0x000000FF00000000ULL) >> 8)
        | ((v & 0x0000FF0000000000ULL) >> 24)
        | ((v & 0x00FF000000000000ULL) >> 40)
        | (v >> 56));
}

inline
void endian_swap(int64 *val)
{
    uint64 v = (uint64) (*val);
    *val = (int64) ((v << 56)
        | ((v & 0x000000000000FF00ULL) << 40)
        | ((v & 0x0000000000FF0000ULL) << 24)
        | ((v & 0x00000000FF000000ULL) << 8)
        | ((v & 0x000000FF00000000ULL) >> 8)
        | ((v & 0x0000FF0000000000ULL) >> 24)
        | ((v & 0x00FF000000000000ULL) >> 40)
        | (v >> 56));
}

#endif