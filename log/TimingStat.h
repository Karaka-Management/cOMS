/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_TIMING_STAT_H
#define TOS_LOG_TIMING_STAT_H

#include <stdio.h>
#include <time.h>
#include "../stdlib/Types.h"
#include "Debug.h"

#if _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

global_persist uint64 performance_count_frequency;

struct TimingStat {
    const char* function;
    uint64 old_tick_count;
    uint64 delta_tick;
    double delta_time;
};

// Sometimes we want to only do logging in debug mode.
// In such cases use the following macro.
#if DEBUG || INTERNAL
    #define UPDATE_TIMING_STAT(stat) update_timing_stat(stat, __func__)
#else
    #define UPDATE_TIMING_STAT(stat) ((void) 0)
#endif

#endif