/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_DEBUG_CONTAINER_H
#define TOS_LOG_DEBUG_CONTAINER_H

#include "Log.h"
#include "DebugMemory.h"
#include "PerformanceProfiler.h"
#include "Stats.h"

struct DebugContainer {
    DebugMemoryContainer* dmc;

    #if _WIN32
        HANDLE log_fp;
    #elif __linux__
        int32 log_fp;
    #endif
    LogMemory* log_memory;

    PerformanceProfileResult* perf_stats;

    atomic_64 int64* stats_counter;

    PerformanceProfiler** perf_current_scope;
};

#endif