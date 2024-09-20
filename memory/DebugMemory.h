/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_DEBUG_H
#define TOS_MEMORY_DEBUG_H

#include <string.h>
#include <malloc.h>

#include "../stdlib/Types.h"

struct DebugMemoryRange {
    uint64 start;
    uint64 end;
};

struct DebugMemory {
    uint64 size;
    uint64 usage;

    uint64 debug_range_size;
    uint64 debug_range_idx;
    DebugMemoryRange* debug_ranges;
};

#if DEBUG
    #define DEBUG_MEMORY(mem, start, end) debug_memory_add_range((mem), (start), (end))
    #define DEBUG_MEMORY_RESET(mem) debug_memory_reset((mem))
    #define DEBUG_MEMORY_FREE(mem, start, end) debug_memory_add_range((mem), (start), (end))
#else
    #define DEBUG_MEMORY(mem, start, end) ((void)0)
    #define DEBUG_MEMORY_RESET(mem) ((void)0)
    #define DEBUG_MEMORY_FREE(mem, start, end) ((void)0)
#endif

void debug_memory_resize(DebugMemory* mem)
{
    DebugMemoryRange* old = mem->debug_ranges;

    mem->debug_range_size += 100;
    mem->debug_ranges = (DebugMemoryRange *) malloc(mem->debug_range_size * sizeof(DebugMemoryRange));

    if (old) {
        memcpy(mem->debug_ranges, old, mem->debug_range_size - 100);
        free(old);
    }
}

void debug_memory_add_range(DebugMemory* mem, uint64 start, uint64 end)
{
    if (mem->debug_range_idx >= mem->debug_range_size) {
        debug_memory_resize(mem);
    }

    mem->debug_ranges[mem->debug_range_idx].start = start;
    mem->debug_ranges[mem->debug_range_idx].end = end;

    ++mem->debug_range_idx;
}

inline
void debug_memory_reset(DebugMemory* mem)
{
    mem->usage = 0;
    mem->debug_range_idx = 0;
}

inline
void debug_memory_free(DebugMemory* mem)
{
    free(mem->debug_ranges);
}

#endif