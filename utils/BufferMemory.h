/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_BUFFER_MEMORY_H
#define TOS_UTILS_BUFFER_MEMORY_H

#include "../stdlib/Types.h"
#include "MathUtils.h"

struct BufferMemory {
    byte* memory;

    uint64 count;
    uint64 element_size;
    uint64 last_pos = -1;

    // length = count
    // free describes which locations are used and which are free
    // @performance using uint32 or even uint64 might be faster
    //      since we can check for free elements faster if the memory is almost filled
    //      at the moment we can only check 8 elements at a time
    byte* free;
};

inline
byte* buffer_element_get(BufferMemory* buf, uint64 element)
{
    return buf->memory + element * buf->element_size;
}

int64 buffer_reserve(BufferMemory* buf)
{
    int byte_index = (buf->last_pos + 1) / 8;
    int bit_index;

    int64 free_element = -1;
    byte mask;

    int i = 0;
    int max_loop = buf->count * buf->element_size;

    while (free_element < 0 && i < max_loop) {
        if (buf->free[byte_index] == 0xFF) {
            ++i;
            ++byte_index;

            continue;
        }

        // This always breaks!
        // @performance on the first iteration through the buffer we could optimize this by starting at a different bit_index
        // because we know that the bit_index is based on last_pos
        for (bit_index = 0; bit_index < 8; ++bit_index) {
            mask = 1 << bit_index;
            if ((buf->free[byte_index] & mask) == 0) {
                free_element = byte_index * 8 + bit_index;
                break;
            }
        }
    }

    if (free_element < 0) {
        return -1;
    }

    buf->free[byte_index] |= (1 << bit_index);

    return byte_index * 8 + bit_index;
}

byte* buffer_find_free(BufferMemory* buf, bool zeroed = false)
{
    int byte_index = (buf->last_pos + 1) / 8;
    int bit_index;

    int64 free_element = -1;
    byte mask;

    int i = 0;
    int max_loop = buf->count * buf->element_size;

    while (free_element < 0 && i < max_loop) {
        if (buf->free[byte_index] == 0xFF) {
            ++i;
            ++byte_index;

            continue;
        }

        // This always breaks!
        // @performance on the first iteration through the buffer we could optimize this by starting at a different bit_index
        // because we know that the bit_index is based on last_pos
        for (bit_index = 0; bit_index < 8; ++bit_index) {
            mask = 1 << bit_index;
            if ((buf->free[byte_index] & mask) == 0) {
                free_element = byte_index * 8 + bit_index;
                break;
            }
        }
    }

    if (free_element < 0) {
        return NULL;
    }

    buf->free[byte_index] |= (1 << bit_index);

    return buf->memory + free_element * buf->element_size;
}

inline
void buffer_element_free(BufferMemory* buf, uint64 element)
{
    int byte_index = element / 8;
    int bit_index = element % 8;

    buf->free[byte_index] &= ~(1 << bit_index);
}

#endif