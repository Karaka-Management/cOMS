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
#include "MathUtils.h"
#include "TestUtils.h"

struct BufferMemory {
    byte* memory;

    uint64 size;
    uint64 pos;
};

inline
byte* buffer_get_memory(BufferMemory* buf, uint64 size, byte aligned = 1, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= buf->size);

    if (aligned > 1 && buf->pos > 0) {
        buf->pos = ROUND_TO_NEAREST(buf->pos, aligned);
    }

    size = ROUND_TO_NEAREST(size, aligned);
    ASSERT_SIMPLE(buf->pos + size <= buf->size);

    byte* offset = (byte *) (buf->memory + buf->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    buf->pos += size;

    return offset;
}

#endif