/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_ELEMENT_MEMORY_H
#define TOS_MEMORY_ELEMENT_MEMORY_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "Allocation.h"
#include "../log/DebugMemory.h"

struct ChunkMemory {
    byte* memory;

    uint64 count;
    uint64 size;
    uint64 chunk_size;
    int64 last_pos;
    int alignment;

    // length = count
    // free describes which locations are used and which are free
    uint64* free;
};

inline
void chunk_alloc(ChunkMemory* buf, uint64 count, uint64 chunk_size, int alignment = 64)
{
    buf->memory = alignment < 2
        ? (byte *) playform_alloc(count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64))
        : (byte *) playform_alloc_aligned(count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64), alignment);

    buf->count = count;
    buf->size = chunk_size * count;
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;
    buf->free = (uint64 *) (buf->memory + count * chunk_size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
}

inline
void chunk_free(ChunkMemory* buf)
{
    DEBUG_MEMORY_DELETE((uint64) buf->memory, buf->size);
    if (buf->alignment < 2) {
        platform_free(buf->memory, buf->size);
    } else {
        platform_aligned_free(buf->memory, buf->size);
    }
}

inline
byte* chunk_get_element(ChunkMemory* buf, uint64 element, bool zeroed = false)
{
    byte* offset = buf->memory + element * buf->chunk_size;

    if (zeroed) {
        memset((void *) offset, 0, buf->chunk_size);
    }

    DEBUG_MEMORY_READ((uint64) offset, buf->chunk_size);

    return offset;
}

/**
 * In some cases we know exactly which index is free
 */
void chunk_reserve_index(ChunkMemory* buf, int64 index, int64 elements = 1, bool zeroed = false)
{
    int64 byte_index = index / 64;
    int bit_index = index % 64;

    // Mark the bits as reserved
    for (int j = 0; j < elements; ++j) {
        int64 current_byte_index = byte_index + (bit_index + j) / 64;
        int current_bit_index = (bit_index + j) % 64;
        buf->free[current_byte_index] |= (1LL << current_bit_index);
    }

    if (zeroed) {
        memset(buf->memory + index * buf->chunk_size, 0, elements * buf->chunk_size);
    }

    DEBUG_MEMORY_WRITE((uint64) (buf->memory + index * buf->chunk_size), elements * buf->chunk_size);

    buf->last_pos = index;
}

int64 chunk_reserve(ChunkMemory* buf, uint64 elements = 1, bool zeroed = false)
{
    int64 byte_index = (buf->last_pos + 1) / 64;
    int bit_index;

    int64 free_element = -1;
    byte mask;

    int i = 0;
    int64 max_bytes = (buf->count + 7) / 64;

    while (free_element < 0 && i < buf->count) {
        ++i;

        if (byte_index >= max_bytes) {
            byte_index = 0;
        }

        if (buf->free[byte_index] == 0xFF) {
            ++byte_index;

            continue;
        }

        // @performance There is some redundancy happening down below, we should ++byte_index in certain conditions?
        for (bit_index = 0; bit_index < 64; ++bit_index) {
            int consecutive_free_bits = 0;

            // Check if there are 'elements' consecutive free bits
            for (int j = 0; j < elements; ++j) {
                uint64 current_byte_index = byte_index + (bit_index + j) / 64;
                int current_bit_index = (bit_index + j) % 64;

                if (current_byte_index >= (buf->count + 7) / 64) {
                    break;
                }

                mask = 1 << current_bit_index;
                if ((buf->free[current_byte_index] & mask) == 0) {
                    ++consecutive_free_bits;
                } else {
                    break;
                }
            }

            if (consecutive_free_bits == elements) {
                free_element = byte_index * 64 + bit_index;

                // Mark the bits as reserved
                for (int j = 0; j < elements; ++j) {
                    int64 current_byte_index = byte_index + (bit_index + j) / 64;
                    int current_bit_index = (bit_index + j) % 64;
                    buf->free[current_byte_index] |= (1LL << current_bit_index);
                }

                break;
            }
        }

        ++i;
        ++byte_index;
    }

    if (free_element < 0) {
        return -1;
    }

    if (zeroed) {
        memset(buf->memory + free_element * buf->chunk_size, 0, elements * buf->chunk_size);
    }

    DEBUG_MEMORY_WRITE((uint64) (buf->memory + free_element * buf->chunk_size), elements * buf->chunk_size);

    buf->last_pos = free_element;

    return free_element;
}

byte* chunk_find_free(ChunkMemory* buf)
{
    int64 byte_index = (buf->last_pos + 1) / 64;
    int bit_index;

    int64 free_element = -1;
    byte mask;

    int i = 0;
    int64 max_bytes = (buf->count + 7) / 64;

    while (free_element < 0 && i < buf->count) {
        if (byte_index >= max_bytes) {
            byte_index = 0;
        }

        if (buf->free[byte_index] == 0xFF) {
            ++i;
            ++byte_index;

            continue;
        }

        // This always breaks!
        // @performance on the first iteration through the buffer we could optimize this by starting at a different bit_index
        // because we know that the bit_index is based on last_pos
        for (bit_index = 0; bit_index < 64; ++bit_index) {
            mask = 1 << bit_index;
            if ((buf->free[byte_index] & mask) == 0) {
                free_element = byte_index * 64 + bit_index;
                buf->free[byte_index] |= (1LL << bit_index);

                break;
            }
        }
    }

    if (free_element < 0) {
        return NULL;
    }

    return buf->memory + free_element * buf->chunk_size;
}

inline
void chunk_free_element(ChunkMemory* buf, uint64 element)
{
    DEBUG_MEMORY_DELETE((uint64) (buf->memory + element * buf->chunk_size), buf->chunk_size);

    int64 byte_index = element / 64;
    int bit_index = element % 64;

    buf->free[byte_index] &= ~(1 << bit_index);
}

#endif