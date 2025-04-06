/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_MEMORY_THREADED_CHUNK_MEMORY_H
#define COMS_MEMORY_THREADED_CHUNK_MEMORY_H

#include "../stdlib/Types.h"
#include "../thread/Thread.h"
#include "ChunkMemory.h"

struct ThreadedChunkMemory {
    byte* memory;

    uint64 size;
    uint32 last_pos;
    uint32 count;
    uint32 chunk_size;
    int32 alignment;

    // length = count
    // free describes which locations are used and which are free
    alignas(8) atomic_64 uint64* free;

    // Chunk implementation ends here
    // The completeness indicates if the data is completely written to
    alignas(8) atomic_64 uint64* completeness;

    mutex lock;
};

// INFO: A chunk count of 2^n is recommended for maximum performance
inline
void thrd_chunk_alloc(ThreadedChunkMemory* buf, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);
    PROFILE(PROFILE_CHUNK_ALLOC, NULL, false, true);
    LOG_1("Allocating ChunkMemory");

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    uint64 size = count * chunk_size
        + sizeof(uint64) * CEIL_DIV(count, alignment) // free
        + sizeof(uint64) * CEIL_DIV(count, alignment) // completeness
        + alignment * 3; // overhead for alignment

    buf->memory = alignment < 2
        ? (byte *) platform_alloc(size)
        : (byte *) platform_alloc_aligned(size, alignment);

    buf->count = count;
    buf->size = size;
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    buf->free = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->memory + count * chunk_size), alignment);
    buf->completeness = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->free + count), alignment);

    memset(buf->memory, 0, buf->size);
    mutex_init(&buf->lock, NULL);

    LOG_1("Allocated ChunkMemory: %n B", {{LOG_DATA_UINT64, &buf->size}});
}

inline
void thrd_chunk_init(ThreadedChunkMemory* buf, BufferMemory* data, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    uint64 size = count * chunk_size
        + sizeof(uint64) * CEIL_DIV(count, alignment) // free
        + sizeof(uint64) * CEIL_DIV(count, alignment) // completeness
        + alignment * 3; // overhead for alignment

    buf->memory = buffer_get_memory(data, size);

    buf->count = count;
    buf->size = size;
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    //  On the other hand the way we do it right now we never have to move past the free array since it is at the end
    //  On another hand we could by accident overwrite the values in free if we are not careful
    buf->free = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->memory + count * chunk_size), alignment);
    buf->completeness = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->free + count), alignment);

    mutex_init(&buf->lock, NULL);

    DEBUG_MEMORY_SUBREGION((uintptr_t) buf->memory, buf->size);
}

inline
void thrd_chunk_init(ThreadedChunkMemory* buf, byte* data, uint32 count, uint32 chunk_size, int32 alignment = 64)
{
    ASSERT_SIMPLE(chunk_size);
    ASSERT_SIMPLE(count);

    chunk_size = ROUND_TO_NEAREST(chunk_size, alignment);

    uint64 size = count * chunk_size
        + sizeof(uint64) * CEIL_DIV(count, alignment) // free
        + sizeof(uint64) * CEIL_DIV(count, alignment) // completeness
        + alignment * 3; // overhead for alignment

    // @bug what if an alignment is defined?
    buf->memory = data;

    buf->count = count;
    buf->size = size;
    buf->chunk_size = chunk_size;
    buf->last_pos = -1;
    buf->alignment = alignment;

    // @question Could it be beneficial to have this before the element data?
    //  On the other hand the way we do it right now we never have to move past the free array since it is at the end
    //  On another hand we could by accident overwrite the values in free if we are not careful
    buf->free = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->memory + count * chunk_size), alignment);
    buf->completeness = (uint64 *) ROUND_TO_NEAREST((uintptr_t) (buf->free + count), alignment);

    mutex_init(&buf->lock, NULL);

    DEBUG_MEMORY_SUBREGION((uintptr_t) buf->memory, buf->size);
}

FORCE_INLINE
void thrd_chunk_free(ThreadedChunkMemory* buf) noexcept
{
    chunk_free((ChunkMemory *) buf);
    mutex_destroy(&buf->lock);
}

FORCE_INLINE
uint32 thrd_chunk_id_from_memory(const ThreadedChunkMemory* buf, const byte* pos) noexcept
{
    return chunk_id_from_memory((ChunkMemory *) buf, pos);
}

FORCE_INLINE
byte* thrd_chunk_get_element(ThreadedChunkMemory* buf, uint32 element, bool zeroed = false) noexcept
{
    return chunk_get_element((ChunkMemory *) buf, element, zeroed);
}

void thrd_chunk_set_unset(uint32 element, atomic_64 uint64* state) {
    uint32 free_index = element / 64;
    uint32 bit_index = element & 63;

    alignas(8) atomic_64 uint64* target = &state[free_index];
    uint64 old_value, new_value;

    do {
        old_value = atomic_get_relaxed(target);
        new_value = old_value | (1ULL << bit_index);

        if (old_value == new_value) {
            return;
        }
    } while (!atomic_compare_exchange_strong_release(target, &old_value, new_value));
}

int32 thrd_chunk_get_unset(ThreadedChunkMemory* buf, atomic_64 uint64* state, int32 start_index = 0) {
    if ((uint32) start_index >= buf->count) {
        start_index = 0;
    }

    uint32 free_index = start_index / 64;
    uint32 bit_index = start_index & 63;

    if (!IS_BIT_SET_64_R2L(state[free_index], bit_index)) {
        uint64 expected = atomic_get_relaxed(&state[free_index]);
        expected &= ~(1ULL << bit_index);
        uint64 desired = expected | (1ULL << bit_index);

        if (atomic_compare_exchange_strong_release(&state[free_index], &expected, desired)) {
            return free_index * 64 + bit_index;
        }
    }

    for (uint32 i = 0; i < buf->count; ++i) {
        if (state[free_index] != 0xFFFFFFFFFFFFFFFF) {
            // We will try 3 times, usually this would be a while but since compiler_find_... doesn't use atomics
            // we might get the same index over and over again
            for (uint32 j = 0; j < 3; ++j) {
                bit_index = compiler_find_first_bit_r2l(~state[free_index]);

                uint64 expected = atomic_get_relaxed(&state[free_index]);
                expected &= ~(1ULL << bit_index);
                uint64 desired = expected | (1ULL << bit_index);

                if (atomic_compare_exchange_strong_release(&state[free_index], &expected, desired)) {
                    return free_index * 64 + bit_index;
                }
            }
        }

        ++free_index;
        if (free_index * 64 >= buf->count) {
            free_index = 0;
        }
    }

    return -1;
}

inline
int32 thrd_chunk_reserve(ThreadedChunkMemory* buf, uint32 elements = 1) noexcept
{
    mutex_lock(&buf->lock);
    int32 free_element = chunk_reserve((ChunkMemory *) buf, elements);
    mutex_unlock(&buf->lock);

    return free_element;
}

inline
void thrd_chunk_free_element(ThreadedChunkMemory* buf, uint64 free_index, int32 bit_index) noexcept
{
    alignas(8) atomic_64 uint64* target = &buf->free[free_index];
    uint64 old_value, new_value;

    do {
        old_value = atomic_get_relaxed(target);
        new_value = old_value | (1ULL << bit_index);

        if (old_value == new_value) {
            return;
        }
    } while (!atomic_compare_exchange_strong_release(target, &old_value, new_value));

    DEBUG_MEMORY_DELETE((uintptr_t) (buf->memory + (free_index * 64 + bit_index) * buf->chunk_size), buf->chunk_size);
}

inline
void thrd_chunk_free_elements(ThreadedChunkMemory* buf, uint64 element, uint32 element_count = 1) noexcept
{
    uint64 free_index = element / 64;
    uint32 bit_index = element & 63;

    if (element == 1) {
        thrd_chunk_free_element(buf, free_index, bit_index);
        return;
    }

    alignas(8) atomic_64 uint64* target;
    uint64 old_value, new_value;

    while (element_count > 0) {
        // Calculate the number of bits we can clear in the current 64-bit block
        uint32 bits_in_current_block = OMS_MIN(64 - bit_index, element_count);

        // Create a mask to clear the bits
        uint64 mask = ((1ULL << bits_in_current_block) - 1) << bit_index;

        target = &buf->free[free_index];

        do {
            old_value = atomic_get_relaxed(target);
            new_value = old_value & ~mask;

            if (old_value == new_value) {
                break;
            }
        } while (!atomic_compare_exchange_strong_release(target, &old_value, new_value));

        // Update the counters and indices
        element_count -= bits_in_current_block;
        ++free_index;
        bit_index = 0;
    }

    DEBUG_MEMORY_DELETE((uintptr_t) (buf->memory + element * buf->chunk_size), buf->chunk_size);
}

inline
int32 thrd_chunk_resize(ThreadedChunkMemory* buf, int32 element_id, uint32 elements_old, uint32 elements_new) noexcept
{
    byte* data = thrd_chunk_get_element(buf, element_id);

    int32 chunk_id = thrd_chunk_reserve(buf, elements_new);
    byte* data_new = thrd_chunk_get_element(buf, chunk_id);

    memcpy(data_new, data, buf->chunk_size * elements_old);

    return chunk_id;
}

#endif