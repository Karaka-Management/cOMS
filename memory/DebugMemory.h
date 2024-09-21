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

// required for __rdtsc
#if _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

struct DebugMemoryRange {
    uint64 start;
    uint64 end;
    uint64 time;
};

struct DebugMemory {
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
    #define DEBUG_MEMORY(mem, start, end) ((void) 0)
    #define DEBUG_MEMORY_RESET(mem) ((void) 0)
    #define DEBUG_MEMORY_FREE(mem, start, end) ((void) 0)
#endif

void debug_memory_resize(DebugMemory* mem)
{
    DebugMemoryRange* old = mem->debug_ranges;

    mem->debug_range_size += 100;
    mem->debug_ranges = (DebugMemoryRange *) calloc(mem->debug_range_size, sizeof(DebugMemoryRange));

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

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->debug_ranges[mem->debug_range_idx].time = __rdtsc();

    ++mem->debug_range_idx;
    mem->usage += end - start;
}

inline
void debug_memory_reset(DebugMemory* mem)
{
    mem->size = 0;
    mem->usage = 0;

    mem->debug_range_idx = 0;
}

inline
void debug_memory_free(DebugMemory* mem)
{
    free(mem->debug_ranges);
}

#endif