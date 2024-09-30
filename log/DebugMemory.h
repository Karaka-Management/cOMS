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

#define DEBUG_MEMORY_RANGE_MAX 1000

struct DebugMemoryRange {
    int32 type;
    uint64 start;
    uint64 size;
    uint64 time;
};

struct DebugMemory {
    uint64 usage;
    uint64 start;
    uint64 size;

    uint64 action_idx;
    DebugMemoryRange last_action[DEBUG_MEMORY_RANGE_MAX];
};

struct DebugMemoryContainer {
    uint64 memory_size;
    uint64 memory_element_idx;
    DebugMemory* memory_stats;
};

#endif