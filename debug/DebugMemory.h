/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_DEBUG_MEMORY_H
#define TOS_LOG_DEBUG_MEMORY_H

#include <string.h>
#include <malloc.h>

#include "../stdlib/Types.h"
#include "Debug.h"

// required for __rdtsc
#if _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

// @todo don't use a pointer to this should be in a global together with other logging data (see Log.h)
inline
DebugMemory* debug_memory_find(uint64 start, uint64 size)
{
    for (int i = 0; i < debug_container->dmc->memory_size; ++i) {
        if (debug_container->dmc->memory_stats[i].start <= start
            && debug_container->dmc->memory_stats[i].start + debug_container->dmc->memory_stats[i].size >= start
        ) {
            return &debug_container->dmc->memory_stats[i];
        }
    }

    return NULL;
}

void debug_memory_init(uint64 start, uint64 size)
{
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
    if (mem) {
        return;
    }

    if (debug_container->dmc->memory_size <= debug_container->dmc->memory_element_idx) {
        DebugMemory* old = debug_container->dmc->memory_stats;

        debug_container->dmc->memory_size += 3;
        debug_container->dmc->memory_stats = (DebugMemory *) calloc(debug_container->dmc->memory_size, sizeof(DebugMemory));

        if (old) {
            memcpy(debug_container->dmc->memory_stats, old, (debug_container->dmc->memory_size - 3) * sizeof(DebugMemory));
            free(old);
        }
    }

    debug_container->dmc->memory_stats[debug_container->dmc->memory_element_idx].start = start;
    debug_container->dmc->memory_stats[debug_container->dmc->memory_element_idx].size = size;
    debug_container->dmc->memory_stats[debug_container->dmc->memory_element_idx].usage = 0;

    ++debug_container->dmc->memory_element_idx;
}

void debug_memory_write(uint64 start, uint64 size)
{
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = 1;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
    mem->usage += size;
}

void debug_memory_read(uint64 start, uint64 size)
{
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = 0;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
}

void debug_memory_delete(uint64 start, uint64 size)
{
    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = -1;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
    mem->usage -= size;
}

inline
void debug_memory_reset()
{
    uint64 time = __rdtsc() - 1000000000;

    for (int i = 0; i < debug_container->dmc->memory_element_idx; ++i) {
        for (int j = 0; j < DEBUG_MEMORY_RANGE_MAX; ++j) {
            if (debug_container->dmc->memory_stats[i].last_action[j].time < time) {
                memset(&debug_container->dmc->memory_stats[i].last_action[j], 0, sizeof(DebugMemoryRange));
            }
        }
    }
}

#if DEBUG
    #define DEBUG_MEMORY_INIT(start, size) debug_memory_init((start), (size))
    #define DEBUG_MEMORY_READ(start, size) debug_memory_read((start), (size))
    #define DEBUG_MEMORY_WRITE(start, size) debug_memory_write((start), (size))
    #define DEBUG_MEMORY_DELETE(start, size) debug_memory_delete((start), (size))
    #define DEBUG_MEMORY_RESET() debug_memory_reset()
#else
    #define DEBUG_MEMORY_INIT(start, size) ((void) 0)
    #define DEBUG_MEMORY_READ(start, size) ((void) 0)
    #define DEBUG_MEMORY_WRITE(start, size) ((void) 0)
    #define DEBUG_MEMORY_DELETE(start, size) ((void) 0)
    #define DEBUG_MEMORY_RESET() ((void) 0)
#endif

#endif