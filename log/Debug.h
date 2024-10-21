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
#include "Log.h"
#include "TimingStat.h"

struct DebugContainer {
    DebugMemoryContainer dmc;

    // Used for logging timings for different sections
    TimingStat* perf_stats;

    // Required to calculate the "fps"
    uint64 performance_count_frequency;

    // Used to log memory access (read, write)
    LogMemory log_memory;

    // Used to log general int values (e.g. counter for draw calls etc.)
    int32* counter;

    #if _WIN32
        HANDLE log_fp;
    #endif
};

#endif