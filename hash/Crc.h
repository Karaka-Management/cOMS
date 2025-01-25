/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_HASH_CRC_H
#define TOS_HASH_CRC_H

#include "../stdlib/Types.h"
#include "../architecture/Intrinsics.h"

inline
uint32 crc32_software_u8(const byte* data, size_t length) {
    uint32 crc = 0xFFFFFFFF;

    // Standard CRC-32 polynomial
    uint32 polynomial = 0xEDB88320;

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i];
        for (byte bit = 0; bit < 8; ++bit) {
            crc = (crc >> 1);

            if (crc & 1) {
                crc ^= polynomial;
            }
        }
    }

    return crc;
}

inline
uint32 crc32_intrin_u8(const byte* data, size_t length) {
    uint32 crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc = intrin_crc32_u8(crc, data[i]);
    }

    return crc;
}

inline
uint32 crc32_intrin_u16(const uint16* data, size_t length) {
    uint32 crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc = intrin_crc32_u16(crc, data[i]);
    }

    return crc;
}

inline
uint32 crc32_intrin_u32(const uint32* data, size_t length) {
    uint32 crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc = intrin_crc32_u32(crc, data[i]);
    }

    return crc;
}

inline
uint32 crc32_intrin_u64(const uint64* data, size_t length) {
    uint64 crc = 0xFFFFFFFF;
    for (size_t i = 0; i < length; ++i) {
        crc = intrin_crc32_u64(crc, data[i]);
    }

    return crc;
}

#endif