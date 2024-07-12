/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_RING_MEMORY_H
#define TOS_UTILS_RING_MEMORY_H

#include "../stdlib/Types.h"
#include "MathUtils.h"
#include "TestUtils.h"

struct RingMemory {
    byte* memory;

    uint64 size;
    uint64 pos;

    // The following two indices are only used in special cases such as iterating through a portion
    // of the ring memory. In such cases it may be necessary to know where the start and end are.
    // Examples for such cases are if a worker thread is pulling data from this ring memory in chunks.
    uint64 start;
    uint64 end;
};

inline
uint64 ring_calculate_position(const RingMemory* ring, uint64 pos, uint64 size, byte aligned = 1)
{
    if (aligned > 1 && ring->pos > 0) {
        pos = ROUND_TO_NEAREST(pos, aligned);
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (pos + size > ring->size) {
        pos = 0;
    }

    return pos;
}

byte* ring_get_memory(RingMemory* ring, uint64 size, byte aligned = 1, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned > 1 && ring->pos > 0) {
        ring->pos = ROUND_TO_NEAREST(ring->pos, aligned);
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (ring->pos + size > ring->size) {
        ring->pos = 0;
    }

    byte* offset = (byte *) ring->memory[ring->pos];
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    ring->pos += size;

    return offset;
}

inline
void ring_reset(RingMemory* ring)
{
    ring->pos = 0;
}

/**
 * Checks if one additional element can be inserted without overwriting the start index
 */
inline
bool ring_commit_safe(const RingMemory* ring, uint64 size, byte aligned = 1)
{
    uint64 pos = ring_calculate_position(ring, ring->pos, size, aligned);

    return ring->start < ring->pos
        ? ring->start < pos
        : pos < ring->start;
}

#endif