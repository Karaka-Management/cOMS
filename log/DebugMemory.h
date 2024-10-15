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

    const char* function_name;
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

#if DEBUG || INTERNAL
    void debug_memory_init(uint64, uint64);
    void debug_memory_read(uint64, uint64, const char*);
    void debug_memory_write(uint64, uint64, const char*);
    void debug_memory_delete(uint64, uint64, const char*);
    void debug_memory_reset();

    #define DEBUG_MEMORY_INIT(start, size) debug_memory_init((start), (size))
    #define DEBUG_MEMORY_READ(start, size) debug_memory_read((start), (size), __func__)
    #define DEBUG_MEMORY_WRITE(start, size) debug_memory_write((start), (size), __func__)
    #define DEBUG_MEMORY_DELETE(start, size) debug_memory_delete((start), (size), __func__)
    #define DEBUG_MEMORY_RESET() debug_memory_reset()
#else
    #define DEBUG_MEMORY_INIT(start, size) ((void) 0)
    #define DEBUG_MEMORY_READ(start, size) ((void) 0)
    #define DEBUG_MEMORY_WRITE(start, size) ((void) 0)
    #define DEBUG_MEMORY_DELETE(start, size) ((void) 0)
    #define DEBUG_MEMORY_RESET() ((void) 0)
#endif

#endif