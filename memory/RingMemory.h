/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_RING_MEMORY_H
#define TOS_MEMORY_RING_MEMORY_H

#include <string.h>
#include <immintrin.h>

#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "../utils/EndianUtils.h"
#include "../utils/TestUtils.h"

#include "BufferMemory.h"
#include "../log/DebugMemory.h"

#if _WIN32
    #include "../platform/win32/Allocator.h"
    #include "../platform/win32/threading/ThreadDefines.h"
    #include "../platform/win32/threading/Semaphore.h"
#elif __linux__
    #include "../platform/linux/Allocator.h"
    #include "../platform/linux/threading/ThreadDefines.h"
    #include "../platform/linux/threading/Semaphore.h"
#endif

struct RingMemory {
    byte* memory;
    byte* end;

    byte* head;

    // This variable is usually only used by single producer/consumer code mostly found in threads.
    // One thread inserts elements -> updates head
    // The other thread reads elements -> updates tail
    // This code itself doesn't change this variable
    byte* tail;

    uint64 size;
    int32 alignment;
    int32 element_alignment;

    // We support both conditional locking and semaphore locking
    // These values are not initialized and not used unless you use the queue
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    sem_t empty;
    sem_t full;
};

// @bug alignment should also include the end point, not just the start

inline
void ring_alloc(RingMemory* ring, uint64 size, int32 alignment = 64)
{
    ASSERT_SIMPLE(size);

    ring->memory = alignment < 2
        ? (byte *) platform_alloc(size)
        : (byte *) platform_alloc_aligned(size, alignment);

    ring->end = ring->memory + size;;
    ring->head = ring->memory;
    ring->tail = ring->memory;
    ring->size = size;
    ring->alignment = alignment;
    ring->element_alignment = 0;

    memset(ring->memory, 0, ring->size);

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
}

inline
void ring_init(RingMemory* ring, BufferMemory* buf, uint64 size, int32 alignment = 64)
{
    ASSERT_SIMPLE(size);

    ring->memory = buffer_get_memory(buf, size, alignment, true);

    ring->end = ring->memory + size;;
    ring->head = ring->memory;
    ring->tail = ring->memory;
    ring->size = size;
    ring->alignment = alignment;
    ring->element_alignment = 0;

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
    DEBUG_MEMORY_RESERVE((uint64) ring->memory, ring->size, 187);
}

inline
void ring_init(RingMemory* ring, byte* buf, uint64 size, int32 alignment = 64)
{
    ASSERT_SIMPLE(size);

    // @bug what if an alignment is defined?
    ring->memory = buf;

    ring->end = ring->memory + size;;
    ring->head = ring->memory;
    ring->tail = ring->memory;
    ring->size = size;
    ring->alignment = alignment;
    ring->element_alignment = 0;

    memset(ring->memory, 0, ring->size);

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
    DEBUG_MEMORY_RESERVE((uint64) ring->memory, ring->size, 187);
}

inline
void ring_free(RingMemory* buf)
{
    if (buf->alignment < 2) {
        platform_free((void **) &buf->memory);
    } else {
        platform_aligned_free((void **) &buf->memory);
    }
}

inline
byte* ring_calculate_position(const RingMemory* ring, uint64 size, byte aligned = 0)
{
    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    byte* head = ring->head;

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) head;
        head += (aligned - (address & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (head + size > ring->end) {
        head = ring->memory;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) head;
            head += (aligned - (address & (aligned - 1))) % aligned;
        }
    }

    return head;
}

inline
void ring_reset(RingMemory* ring)
{
    DEBUG_MEMORY_DELETE((uint64) ring->memory, ring->size);
    ring->head = ring->memory;
}

// Moves a pointer based on the size you want to consume (new position = after consuming size)
void ring_move_pointer(RingMemory* ring, byte** pos, uint64 size, byte aligned = 0)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) *pos;
        *pos += (aligned - (address& (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (*pos + size > ring->end) {
        *pos = ring->memory;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) *pos;
            *pos += (aligned - (address & (aligned - 1))) % aligned;
        }
    }

    *pos += size;
}

byte* ring_get_memory(RingMemory* ring, uint64 size, byte aligned = 0, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) ring->head;
        ring->head += (aligned - (address& (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (ring->head + size > ring->end) {
        ring_reset(ring);

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->head;
            ring->head += (aligned - (address & (aligned - 1))) % aligned;
        }
    }

    if (zeroed) {
        memset((void *) ring->head, 0, size);
    }

    DEBUG_MEMORY_WRITE((uint64) ring->head, size);

    byte* offset = ring->head;
    ring->head += size;

    ASSERT_SIMPLE(offset);

    return offset;
}

// Same as ring_get_memory but DOESN'T move the head
byte* ring_get_memory_nomove(RingMemory* ring, uint64 size, byte aligned = 0, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    byte* pos = ring->head;

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) pos;
        pos += (aligned - (address& (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (pos + size > ring->end) {
        ring_reset(ring);

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) pos;
            pos += (aligned - (address & (aligned - 1))) % aligned;
        }
    }

    if (zeroed) {
        memset((void *) pos, 0, size);
    }

    DEBUG_MEMORY_WRITE((uint64) pos, size);

    return pos;
}

// Used if the ring only contains elements of a certain size
// This way you can get a certain element
inline
byte* ring_get_element(const RingMemory* ring, uint64 element_count, uint64 element, uint64 size)
{
    int64 index = (element % element_count) - 1;

    DEBUG_MEMORY_READ((uint64) (ring->memory + index * size), 1);

    return ring->memory + index * size;
}

/**
 * Checks if one additional element can be inserted without overwriting the tail index
 */
inline
bool ring_commit_safe(const RingMemory* ring, uint64 size, byte aligned = 0)
{
    // aligned * 2 since that should be the maximum overhead for an element
    // @bug could this result in a case where the ring is considered empty/full (false positive/negative)?
    // The "correct" version would probably to use ring_move_pointer in some form
    uint64 max_mem_required = size + aligned * 2;

    if (ring->tail < ring->head) {
        return ((uint64) (ring->end - ring->head)) > max_mem_required
            || ((uint64) (ring->tail - ring->memory)) > max_mem_required;
    } else if (ring->tail > ring->head) {
        return ((uint64) (ring->tail - ring->head)) > max_mem_required;
    } else {
        return true;
    }
}

inline
void ring_force_head_update(const RingMemory* ring)
{
    _mm_clflush(ring->head);
}

inline
void ring_force_tail_update(const RingMemory* ring)
{
    _mm_clflush(ring->tail);
}

inline
int64 ring_dump(const RingMemory* ring, byte* data)
{
    byte* start = data;

    // Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(ring->size);
    data += sizeof(ring->size);

    // head
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uint64) (ring->head - ring->memory));
    data += sizeof(ring->head);

    // Alignment
    *((int32 *) data) = SWAP_ENDIAN_LITTLE(ring->alignment);
    data += sizeof(ring->alignment);

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(ring->element_alignment);
    data += sizeof(ring->element_alignment);

    // tail/End
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uint64) (ring->tail - ring->memory));
    data += sizeof(ring->tail);

    *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uint64) (ring->end - ring->memory));
    data += sizeof(ring->end);

    // All memory is handled in the buffer -> simply copy the buffer
    memcpy(data, ring->memory, ring->size);
    data += ring->size;

    return data - start;
}

#endif