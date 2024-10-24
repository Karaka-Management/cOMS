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
#include "../utils/EndianUtils.h"
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
    buf->element_alignment = 0;
    buf->size = size;

    memset(buf->memory, 0, buf->size);

    DEBUG_MEMORY_INIT((uint64) buf->memory, size);
}

inline
void buffer_free(BufferMemory* buf)
{
    DEBUG_MEMORY_DELETE((uint64) buf->memory, buf->size);
    if (buf->alignment < 2) {
        platform_free((void **) &buf->memory, buf->size);
    } else {
        platform_aligned_free((void **) &buf->memory, buf->size);
    }
}

inline
void buffer_init(BufferMemory* buf, byte* data, uint64 size, int32 alignment = 64)
{
    // @bug what if an alignment is defined?
    buf->memory = data;

    buf->size = size;
    buf->pos = 0;
    buf->alignment = alignment;
    buf->element_alignment = 0;

    DEBUG_MEMORY_INIT((uint64) buf->memory, buf->size);
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

inline
int64 buffer_dump(const BufferMemory* buf, byte* data)
{
    byte* start = data;

    // Size
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->size);
    data += sizeof(buf->size);

    // Pos
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(buf->pos);
    data += sizeof(buf->pos);

    // Alignment
    *((int32 *) data) = SWAP_ENDIAN_LITTLE(buf->alignment);
    data += sizeof(buf->alignment);

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(buf->element_alignment);
    data += sizeof(buf->element_alignment);

    // All memory is handled in the buffer -> simply copy the buffer
    memcpy(data, buf->memory, buf->size);
    data += buf->size;

    return data - start;
}

inline
int64 buffer_load(BufferMemory* buf, const byte* data)
{
    // Size
    buf->size = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->size);

    // Pos
    buf->pos = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(buf->pos);

    // Alignment
    buf->alignment = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(buf->alignment);

    buf->element_alignment = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(buf->element_alignment);

    memcpy(buf->memory, data, buf->size);
    data += buf->size;
}

#endif