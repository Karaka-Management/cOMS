/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_DEBUG_H
#define TOS_LOG_DEBUG_H

#include "../stdlib/Types.h"
#include "DebugMemory.h"
#include "TimingStat.h"

#if _WIN32
    #include <windows.h>
    #include "../platform/win32/threading/Spinlock.h"
#elif __linux__
    #include "../platform/linux/threading/Spinlock.h"
#endif

struct LogMemory {
    byte* memory;

    uint32 id;
    uint64 size;
    uint64 pos;
    int32 alignment;
    uint64 start;
    uint64 end;
};

struct DebugContainer {
    DebugMemoryContainer dmc;

    // Used for logging timings for different sections
    TimingStat* perf_stats;
    spinlock32 perf_stats_spinlock;

    // Required to calculate the "fps"
    uint64 performance_count_frequency;

    // Used to log memory access (read, write)
    LogMemory log_memory;

    // Used to log general int values (e.g. counter for draw calls etc.)
    int32* counter;

    #if _WIN32
        HANDLE log_fp;
    #elif __linux__
        int32 log_fp;
    #endif
};

#endif