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
    TimingStat* perf_stats;
    LogMemory log_memory;

    #if _WIN32
        HANDLE log_fp;
    #endif
};

#endif