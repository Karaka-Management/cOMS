/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_BIT_H
#define TOS_UTILS_BIT_H

#include "../stdlib/Types.h"

uint32 bytes_merge(byte b0, byte b1, byte b2, byte b3) {
    uint32 result = 0;

    result |= ((uint32) b0 << 24);
    result |= ((uint32) b1 << 16);
    result |= ((uint32) b2 << 8);
    result |= (uint32) b3;

    return result;
}

uint64 bytes_merge(
    byte b0, byte b1, byte b2, byte b3,
    byte b4, byte b5, byte b6, byte b7
) {
    uint64 result = 0;

    result |= ((uint32) b0 << 56);
    result |= ((uint32) b1 << 48);
    result |= ((uint32) b2 << 40);
    result |= ((uint32) b3 << 32);
    result |= ((uint32) b4 << 24);
    result |= ((uint32) b5 << 16);
    result |= ((uint32) b6 << 8);
    result |= (uint32) b3;

    return result;
}

#endif