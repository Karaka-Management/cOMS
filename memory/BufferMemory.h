/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_BUFFER_MEMORY_H
#define TOS_MEMORY_BUFFER_MEMORY_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/MathUtils.h"
#include "../utils/TestUtils.h"
#include "Allocation.h"
#include "../log/DebugMemory.h"

// @question Consider to use element_alignment to automatically align/pad elmeents

struct BufferMemory {
    byte* memory;

    uint64 size;
    uint64 pos;
    int32 alignment;
    int32 element_alignment;
};

inline
void buffer_alloc(BufferMemory* buf, uint64 size, int32 alignment = 64)
{
    buf->memory = alignment < 2
        ? (byte *) playform_alloc(size)
        : (byte *) playform_alloc_aligned(size, alignment);

    buf->alignment = alignment;
    buf->size = size;

    DEBUG_MEMORY_INIT((uint64) buf->memory, size);
}

inline
void buffer_free(BufferMemory* buf)
{
    DEBUG_MEMORY_DELETE((uint64) buf->memory, buf->size);
    if (buf->alignment < 2) {
        platform_free(buf->memory, buf->size);
    } else {
        platform_aligned_free(buf->memory, buf->size);
    }
}

inline
void buffer_reset(BufferMemory* buf)
{
    // @bug arent we wasting element 0 (see get_memory, we are not using 0 only next element)
    DEBUG_MEMORY_DELETE((uint64) buf->memory, buf->pos);
    buf->pos = 0;
}

inline
byte* buffer_get_memory(BufferMemory* buf, uint64 size, int32 aligned = 0, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= buf->size);

    if (aligned == 0) {
        aligned = (byte) OMS_MAX(buf->element_alignment, 1);
    }

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) buf->memory;
        buf->pos += (aligned - ((address + buf->pos) & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    ASSERT_SIMPLE(buf->pos + size <= buf->size);

    byte* offset = (byte *) (buf->memory + buf->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    DEBUG_MEMORY_WRITE((uint64) offset, size);

    buf->pos += size;

    return offset;
}

#endif