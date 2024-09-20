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
#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "../utils/TestUtils.h"
#include "Allocation.h"
#include "BufferMemory.h"
#include "DebugMemory.h"

struct RingMemory {
    byte* memory;

    uint32 id;
    uint64 size;
    uint64 pos;
    int alignment;

    // The following two indices are only used in special cases such as iterating through a portion
    // of the ring memory. In such cases it may be necessary to know where the start and end are.
    // Examples for such cases are if a worker thread is pulling data from this ring memory in chunks.

    // @question Is it guaranteed that if a thread realizes end changed, that also the memory is changed
    //      or is it possible that end changed but it still has old *memory in the cache?
    //      if yes we need to also check and wait for *memory != NULL and obviously set the memory to NULL
    //      after using it.
    uint64 start;
    uint64 end;
};

// @todo implement memory usage visualization

inline
void ring_alloc(RingMemory* ring, uint64 size, int alignment = 1)
{
    ring->memory = alignment < 2
        ? (byte *) playform_alloc(size)
        : (byte *) playform_alloc_aligned(size, alignment);

    ring->size = size;
    ring->pos = 0;
    ring->alignment = alignment;
    ring->start = 0;
    ring->end = 0;
}

inline
void ring_create(RingMemory* ring, BufferMemory* buf, uint64 size, int alignment = 1)
{
    ring->memory = buffer_get_memory(buf, size, alignment);

    ring->size = size;
    ring->pos = 0;
    ring->alignment = alignment;
    ring->start = 0;
    ring->end = 0;
}

inline
void ring_free(RingMemory* buf)
{
    if (buf->alignment < 2) {
        platform_free(buf->memory, buf->size);
    } else {
        platform_aligned_free(buf->memory, buf->size);
    }
}

inline
uint64 ring_calculate_position(const RingMemory* ring, uint64 pos, uint64 size, byte aligned = 1)
{
    if (aligned) {
        uintptr_t address = (uintptr_t) ring->memory;
        int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

        pos += adjustment;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (pos + size > ring->size) {
        pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->memory;
            int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

            pos += adjustment;
        }
    }

    return pos;
}

byte* ring_get_memory(RingMemory* ring, uint64 size, byte aligned = 1, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) ring->memory;
        int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

        ring->pos += adjustment;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (ring->pos + size > ring->size) {
        ring->pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->memory;
            int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

            ring->pos += adjustment;
        }
    }

    byte* offset = (byte *) (ring->memory + ring->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    DEBUG_MEMORY(&debug_memory[ring->id], ring->pos, size);

    ring->pos += size;

    return offset;
}

// Used if the ring only contains elements of a certain size
// This way you can get a certain element
inline
byte* ring_get_element(const RingMemory* ring, uint64 element_count, uint64 element, uint64 size)
{
    int64 index = (element % element_count) - 1;
    return ring->memory + index * size;
}

inline
void ring_reset(RingMemory* ring)
{
    ring->pos = 0;
    DEBUG_MEMORY_RESET(&debug_memory[ring->id]);
}

/**
 * Checks if one additional element can be inserted without overwriting the start index
 */
inline
bool ring_commit_safe(const RingMemory* ring, uint64 size, byte aligned = 1)
{
    uint64 pos = ring_calculate_position(ring, ring->pos, size, aligned);

    if (ring->start == ring->end && ring->pos == 0) {
        return true;
    }

    return ring->start < ring->pos
        ? ring->start < pos
        : pos < ring->start;
}

#endif