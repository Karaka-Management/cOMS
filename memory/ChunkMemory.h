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
#include "../log/DebugMemory.h"
#include "BufferMemory.h"

#if _WIN32
    #include "../platform/win32/Allocator.h"
#elif __linux__
    #include "../platform/linux/Allocator.h"
#endif

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
#endif

struct ChunkMemory {
    byte* memory;

    uint64 count;
    uint64 size;
    uint64 chunk_size;
    int64 last_pos;
    int32 alignment;

    // length = count
    // free describes which locations are used and which are free
    uint64* free;
};

struct ThreadedChunkMemory {
    byte* memory;

    uint64 count;
    uint64 size;
    uint64 chunk_size;
    int64 last_pos;
    int32 alignment;

    // length = count
    // free describes which locations are used and which are free
    uint64* free;

    pthread_mutex_t mutex;
    pthread_cond_t cond;
};

inline
void chunk_alloc(ChunkMemory* buf, uint64 count, uint64 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    buf->memory = alignment < 2
        ? (byte *) platform_alloc(count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64))
        : (byte *) platform_alloc_aligned(count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64), alignment);

    buf->count = count;
    buf->size = count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64);
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    buf->free = (uint64 *) (buf->memory + count * chunk_size);

    memset(buf->memory, 0, buf->size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
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
void chunk_init(ChunkMemory* buf, BufferMemory* data, uint64 count, uint64 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    // @bug what if an alignment is defined?
    buf->memory = buffer_get_memory(data, count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64));

    buf->count = count;
    buf->size = count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64);
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
void chunk_init(ChunkMemory* buf, byte* data, uint64 count, uint64 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    // @bug what if an alignment is defined?
    buf->memory = data;

    buf->count = count;
    buf->size = count * chunk_size + sizeof(buf->free) * CEIL_DIV(count, 64);
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
byte* chunk_get_element(ChunkMemory* buf, uint64 element, bool zeroed = false)
{
    byte* offset = buf->memory + element * buf->chunk_size;

    if (zeroed) {
        memset((void *) offset, 0, buf->chunk_size);
    }

    DEBUG_MEMORY_READ((uint64) offset, buf->chunk_size);

    ASSERT_SIMPLE(offset);

    return offset;
}

/**
 * In some cases we know exactly which index is free
 */
void chunk_reserve_index(ChunkMemory* buf, int64 index, int64 elements = 1, bool zeroed = false)
{
    int64 byte_index = index / 64;
    int32 bit_index = index % 64;

    // Mark the bits as reserved
    for (int32 j = 0; j < elements; ++j) {
        int64 current_byte_index = byte_index + (bit_index + j) / 64;
        int32 current_bit_index = (bit_index + j) % 64;
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
    int64 free_index = (buf->last_pos + 1) / 64;
    int32 bit_index;

    int64 free_element = -1;
    int64 mask;

    int32 i = 0;
    int64 max_bytes = (buf->count + 7) / 64;

    while (free_element < 0 && i < buf->count) {
        ++i;

        if (free_index >= max_bytes) {
            free_index = 0;
        }

        if (buf->free[free_index] == 0xFF) {
            ++free_index;

            continue;
        }

        // @performance There is some redundancy happening down below, we should ++free_index in certain conditions?
        for (bit_index = 0; bit_index < 64; ++bit_index) {
            int32 consecutive_free_bits = 0;

            // Check if there are 'elements' consecutive free bits
            for (int32 j = 0; j < elements; ++j) {
                // Check if there is enough space until the end of the buffer.
                // Remember, the last free index may only allow only 1 bit if the size is 65
                if (free_index * 64 + (bit_index + j) >= buf->count) {
                    break;
                }

                uint64 current_free_index = free_index + (bit_index + j) / 64;
                int32 current_bit_index = (bit_index + j) % 64;

                mask = 1LL << current_bit_index;
                if ((buf->free[current_free_index] & mask) == 0) {
                    ++consecutive_free_bits;
                } else {
                    break;
                }
            }

            if (consecutive_free_bits == elements) {
                free_element = free_index * 64 + bit_index;

                // Mark the bits as reserved
                for (int32 j = 0; j < elements; ++j) {
                    int64 current_free_index = free_index + (bit_index + j) / 64;
                    int32 current_bit_index = (bit_index + j) % 64;
                    buf->free[current_free_index] |= (1LL << current_bit_index);
                }

                break;
            }
        }

        ++i;
        ++free_index;
    }

    if (free_element < 0) {
        ASSERT_SIMPLE(false);
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
    int64 free_index = (buf->last_pos + 1) / 64;
    int32 bit_index;

    int64 free_element = -1;
    int64 mask;

    int32 i = 0;
    int64 max_bytes = (buf->count + 7) / 64;

    while (free_element < 0 && i < buf->count) {
        if (free_index >= max_bytes) {
            free_index = 0;
        }

        if (buf->free[free_index] == 0xFF) {
            ++i;
            ++free_index;

            continue;
        }

        // This always breaks!
        // @performance on the first iteration through the buffer we could optimize this by starting at a different bit_index
        // because we know that the bit_index is based on last_pos
        for (bit_index = 0; bit_index < 64; ++bit_index) {
            mask = 1LL << bit_index;
            if ((buf->free[free_index] & mask) == 0) {
                free_element = free_index * 64 + bit_index;
                buf->free[free_index] |= (1LL << bit_index);

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

    int64 free_index = element / 64;
    int32 bit_index = element % 64;

    buf->free[free_index] &= ~(1LL << bit_index);
}

inline
int64 chunk_dump(const ChunkMemory* buf, byte* data)
{
    byte* start = data;

    // Count
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->count);
    data += sizeof(buf->count);

    // Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->size);
    data += sizeof(buf->size);

    // Chunk Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->chunk_size);
    data += sizeof(buf->chunk_size);

    // Last pos
    *((int64 *) data) = SWAP_ENDIAN_LITTLE(buf->last_pos);
    data += sizeof(buf->last_pos);

    // Alignment
    *((int32 *) data) = SWAP_ENDIAN_LITTLE(buf->alignment);
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
    buf->count = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->count);

    // Size
    buf->size = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->size);

    // Chunk Size
    buf->chunk_size = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->chunk_size);

    // Last pos
    buf->last_pos = SWAP_ENDIAN_LITTLE(*((int64 *) data));
    data += sizeof(buf->last_pos);

    // Alignment
    buf->alignment = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(buf->alignment);

    memcpy(buf->memory, data, buf->size);
    data += buf->size;

    buf->free = (uint64 *) (buf->memory + buf->count * buf->chunk_size);

    return buf->size;
}

#endif