/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_CHUNK_MEMORY_H
#define TOS_MEMORY_CHUNK_MEMORY_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "../utils/TestUtils.h"
#include "../utils/EndianUtils.h"
#include "../utils/BitUtils.h"
#include "../log/DebugMemory.h"
#include "BufferMemory.h"
#include "../system/Allocator.h"
#include "../thread/Thread.h"

struct ChunkMemory {
    byte* memory;

    // @question Why are we making the count 64 bit? is this really realistically possible?
    uint64 size;
    int32 last_pos;
    uint32 count;
    uint32 chunk_size;
    uint32 alignment;

    // length = count
    // free describes which locations are used and which are free
    uint64* free;
};

inline
void chunk_alloc(ChunkMemory* buf, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    buf->memory = alignment < 2
        ? (byte *) platform_alloc(count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64))
        : (byte *) platform_alloc_aligned(count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64), alignment);

    buf->count = count;
    buf->size = count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64);
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    buf->free = (uint64 *) (buf->memory + count * chunk_size);

    memset(buf->memory, 0, buf->size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
}

inline
void chunk_init(ChunkMemory* buf, BufferMemory* data, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    buf->memory = buffer_get_memory(data, count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64));

    buf->count = count;
    buf->size = count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64);
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    //  On the other hand the way we do it right now we never have to move past the free array since it is at the end
    //  On another hand we could by accident overwrite the values in free if we are not careful
    buf->free = (uint64 *) (buf->memory + count * chunk_size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
    DEBUG_MEMORY_RESERVE((uint64) buf->memory, buf->size, 187);
}

inline
void chunk_init(ChunkMemory* buf, byte* data, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    // @bug what if an alignment is defined?
    buf->memory = data;

    buf->count = count;
    buf->size = count * chunk_size + sizeof(uint64) * CEIL_DIV(count, 64);
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    //  On the other hand the way we do it right now we never have to move past the free array since it is at the end
    //  On another hand we could by accident overwrite the values in free if we are not careful
    buf->free = (uint64 *) (buf->memory + count * chunk_size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
    DEBUG_MEMORY_RESERVE((uint64) buf->memory, buf->size, 187);
}

inline
void chunk_free(ChunkMemory* buf)
{
    DEBUG_MEMORY_DELETE((uint64) buf->memory, buf->size);
    if (buf->alignment < 2) {
        platform_free((void **) &buf->memory);
    } else {
        platform_aligned_free((void **) &buf->memory);
    }
}

inline
uint32 chunk_id_from_memory(ChunkMemory* buf, byte* pos) {
    return (uint32) ((uintptr_t) pos - (uintptr_t) buf->memory) / buf->chunk_size;
}

inline
byte* chunk_get_element(ChunkMemory* buf, uint64 element, bool zeroed = false)
{
    byte* offset = buf->memory + element * buf->chunk_size;
    ASSERT_SIMPLE(offset);

    if (zeroed) {
        memset((void *) offset, 0, buf->chunk_size);
    }

    DEBUG_MEMORY_READ((uint64) offset, buf->chunk_size);

    return offset;
}

// @performance This is a very important function, revisit in the future for optimization (e.g. ABM)
int32 chunk_reserve(ChunkMemory* buf, uint32 elements = 1)
{
    int32 free_index = (buf->last_pos + 1) / 64;
    int32 bit_index = (buf->last_pos + 1) & 63;
    int32 free_element = -1;

    int32 i = -1;
    int32 consecutive_free_bits = 0;

    while (free_element < 0 && ++i < buf->count) {
        // Skip fully filled ranges
        if (free_index * 64 + bit_index + elements - consecutive_free_bits >= buf->count) {
            free_index = 0;
            bit_index = 0;
            i += buf->count - (free_index * 64 + bit_index);
            consecutive_free_bits = 0;
        } else if (buf->free[free_index] == 0xFFFFFFFFFFFFFFFF) {
            ++free_index;
            bit_index = 0;
            i += 63;
            consecutive_free_bits = 0;

            continue;
        }

        // Find first free element
        while (IS_BIT_SET_64_R2L(buf->free[free_index], bit_index)) {
            consecutive_free_bits = 0;
            ++bit_index;
            ++i;

            // We still need to check for overflow since our initial bit_index is based on buf->last_pos
            if (bit_index > 63) {
                bit_index = 0;
                ++free_index;

                break;
            }
        }

        // The previous while may exit with an "overflow", that's why this check is required
        if (IS_BIT_SET_64_R2L(buf->free[free_index], bit_index)) {
            consecutive_free_bits = 0;

            continue;
        }

        // We found our first free element, let's check if we have enough free space
        while (!IS_BIT_SET_64_R2L(buf->free[free_index], bit_index)
            && consecutive_free_bits != elements
            && free_index * 64 + bit_index + elements - consecutive_free_bits < buf->count
        ) {
            ++i;
            ++consecutive_free_bits;
            ++bit_index;

            if (bit_index > 63) {
                bit_index = 0;
                ++free_index;

                break;
            }
        }

        // Do we have enough free bits?
        if (consecutive_free_bits == elements) {
            free_element = free_index * 64 + bit_index - elements;
            int32 possible_free_index = free_element / 64;
            int32 possible_bit_index = free_element & 63;

            // Mark as used
            if (elements == 1) {
                buf->free[possible_free_index] |= (1LL << possible_bit_index);
            } else {
                uint32 elements_temp = elements;
                int64 current_free_index = possible_free_index;
                int32 current_bit_index = possible_bit_index;

                while (elements > 0) {
                    // Calculate the number of bits we can set in the current 64-bit block
                    int32 bits_in_current_block = OMS_MIN(64 - current_bit_index, elements);

                    // Create a mask to set the bits
                    uint64 mask = ((1ULL << bits_in_current_block) - 1) << current_bit_index;
                    buf->free[current_free_index] |= mask;

                    // Update the counters and indices
                    elements -= bits_in_current_block;
                    ++current_free_index;
                    current_bit_index = 0;
                }
            }

            break;
        }
    }

    if (free_element < 0) {
        ASSERT_SIMPLE(false);
        return -1;
    }

    DEBUG_MEMORY_WRITE((uint64) (buf->memory + free_element * buf->chunk_size), elements * buf->chunk_size);

    buf->last_pos = free_element;

    return (int32) free_element;
}

inline
void chunk_free_element(ChunkMemory* buf, uint64 free_index, int32 bit_index)
{
    DEBUG_MEMORY_DELETE((uint64) (buf->memory + (free_index * 64 + bit_index) * buf->chunk_size), buf->chunk_size);
    buf->free[free_index] &= ~(1LL << bit_index);
}

inline
void chunk_free_elements(ChunkMemory* buf, uint64 element, uint32 element_count = 1)
{
    DEBUG_MEMORY_DELETE((uint64) (buf->memory + element * buf->chunk_size), buf->chunk_size);

    int64 free_index = element / 64;
    int32 bit_index = element & 63;

    if (element == 1) {
        chunk_free_element(buf, free_index, bit_index);
        return;
    }

    while (element_count > 0) {
        // Calculate the number of bits we can clear in the current 64-bit block
        uint32 bits_in_current_block = OMS_MIN(64 - bit_index, element_count);

        // Create a mask to clear the bits
        uint64 mask = ((1ULL << bits_in_current_block) - 1) << bit_index;
        buf->free[free_index] &= ~mask;

        // Update the counters and indices
        element_count -= bits_in_current_block;
        ++free_index;
        bit_index = 0;
    }
}

inline
int64 chunk_dump(const ChunkMemory* buf, byte* data)
{
    byte* start = data;

    // Count
    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(buf->count);
    data += sizeof(buf->count);

    // Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->size);
    data += sizeof(buf->size);

    // Chunk Size
    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(buf->chunk_size);
    data += sizeof(buf->chunk_size);

    // Last pos
    *((int32 *) data) = SWAP_ENDIAN_LITTLE(buf->last_pos);
    data += sizeof(buf->last_pos);

    // Alignment
    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(buf->alignment);
    data += sizeof(buf->alignment);

    // All memory is handled in the buffer -> simply copy the buffer
    // This also includes the free array
    memcpy(data, buf->memory, buf->size);
    data += buf->size;

    return data - start;
}

inline
int64 chunk_load(ChunkMemory* buf, const byte* data)
{
    // Count
    buf->count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(buf->count);

    // Size
    buf->size = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->size);

    // Chunk Size
    buf->chunk_size = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(buf->chunk_size);

    // Last pos
    buf->last_pos = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(buf->last_pos);

    // Alignment
    buf->alignment = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(buf->alignment);

    memcpy(buf->memory, data, buf->size);
    data += buf->size;

    buf->free = (uint64 *) (buf->memory + buf->count * buf->chunk_size);

    return buf->size;
}

#define chunk_iterate_start(buf, chunk_id)                                                      \
    int32 free_index = 0;                                                                       \
    int32 bit_index = 0;                                                                        \
                                                                                                \
    /* Iterate the chunk memory */                                                              \
    for (; chunk_id < (buf)->count; ++chunk_id) {                                               \
        /* Check if asset is defined */                                                         \
        if (!(buf)->free[free_index]) {                                                         \
            /* Skip various elements */                                                         \
            /* @performance Consider to only check 1 byte instead of 8 */                       \
            /* There are probably even better ways by using compiler intrinsics if available */ \
            bit_index += 63; /* +64 - 1 since the loop also increases by 1 */                   \
        } else if ((buf)->free[free_index] & (1ULL << bit_index)) {

#define chunk_iterate_end       \
        }                       \
                                \
        ++bit_index;            \
        if (bit_index > 63) {   \
            bit_index = 0;      \
            ++free_index;       \
        }                       \
    }

#endif