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
#include "../utils/EndianUtils.h"
#include "../utils/TestUtils.h"

#include "Allocation.h"
#include "BufferMemory.h"
#include "../log/DebugMemory.h"

struct RingMemory {
    byte* memory;

    uint64 size;
    uint64 pos;
    int32 alignment;
    int32 element_alignment;

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

inline
void ring_alloc(RingMemory* ring, uint64 size, int32 alignment = 64)
{
    ring->memory = alignment < 2
        ? (byte *) playform_alloc(size)
        : (byte *) playform_alloc_aligned(size, alignment);

    ring->size = size;
    ring->pos = 0;
    ring->alignment = alignment;
    ring->element_alignment = 0;
    ring->start = 0;
    ring->end = 0;

    memset(ring->memory, 0, ring->size);

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
}

inline
void ring_init(RingMemory* ring, BufferMemory* buf, uint64 size, int32 alignment = 64)
{
    ring->memory = buffer_get_memory(buf, size, alignment, true);

    ring->size = size;
    ring->pos = 0;
    ring->alignment = alignment;
    ring->element_alignment = 0;
    ring->start = 0;
    ring->end = 0;

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
}

inline
void ring_init(RingMemory* ring, byte* buf, uint64 size, int32 alignment = 64)
{
    // @bug what if an alignment is defined?
    ring->memory = buf;

    ring->size = size;
    ring->pos = 0;
    ring->alignment = alignment;
    ring->element_alignment = 0;
    ring->start = 0;
    ring->end = 0;

    memset(ring->memory, 0, ring->size);

    DEBUG_MEMORY_INIT((uint64) ring->memory, ring->size);
}

inline
void ring_free(RingMemory* buf)
{
    if (buf->alignment < 2) {
        platform_free((void **) &buf->memory, buf->size);
    } else {
        platform_aligned_free((void **) &buf->memory, buf->size);
    }
}

inline
uint64 ring_calculate_position(const RingMemory* ring, uint64 pos, uint64 size, byte aligned = 0)
{
    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    if (aligned) {
        uintptr_t address = (uintptr_t) ring->memory;
        pos += (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (pos + size > ring->size) {
        pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->memory;
            pos += (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;
        }
    }

    return pos;
}

inline
void ring_reset(RingMemory* ring)
{
    DEBUG_MEMORY_DELETE((uint64) ring->memory, ring->size);
    ring->pos = 0;
}

byte* ring_get_memory(RingMemory* ring, uint64 size, byte aligned = 0, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned == 0) {
        aligned = (byte) OMS_MAX(ring->element_alignment, 1);
    }

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) ring->memory;
        ring->pos += (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (ring->pos + size > ring->size) {
        ring_reset(ring);

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->memory;
            ring->pos += (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;
        }
    }

    byte* offset = (byte *) (ring->memory + ring->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    DEBUG_MEMORY_WRITE((uint64) offset, size);

    ring->pos += size;

    return offset;
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
 * Checks if one additional element can be inserted without overwriting the start index
 */
inline
bool ring_commit_safe(const RingMemory* ring, uint64 size, byte aligned = 0)
{
    uint64 pos = ring_calculate_position(ring, ring->pos, size, aligned);

    if (ring->start == ring->end && ring->pos == 0) {
        return true;
    }

    return ring->start < ring->pos
        ? ring->start < pos
        : pos < ring->start;
}

inline
int64 ring_dump(const RingMemory* ring, byte* data)
{
    byte* start = data;

    // Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(ring->size);
    data += sizeof(ring->size);

    // Pos
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(ring->pos);
    data += sizeof(ring->pos);

    // Alignment
    *((int32 *) data) = SWAP_ENDIAN_LITTLE(ring->alignment);
    data += sizeof(ring->alignment);

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(ring->element_alignment);
    data += sizeof(ring->element_alignment);

    // Start/End
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(ring->start);
    data += sizeof(ring->start);

    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(ring->end);
    data += sizeof(ring->end);

    // All memory is handled in the buffer -> simply copy the buffer
    memcpy(data, ring->memory, ring->size);
    data += ring->size;

    return data - start;
}

#endif