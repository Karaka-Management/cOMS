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

// IMPORTANT: This function should only be called when you actually use this data
//      e.g. log to display or file
inline
void update_timing_stat(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    debug_container->perf_stats[stat].function = function;
    debug_container->perf_stats[stat].delta_tick = new_tick_count - debug_container->perf_stats[stat].old_tick_count;
    debug_container->perf_stats[stat].delta_time = (double) debug_container->perf_stats[stat].delta_tick / (double) performance_count_frequency;
    debug_container->perf_stats[stat].old_tick_count = new_tick_count;
}

// Sometimes we want to only do logging in debug mode.
// In such cases use the following macro.
#if DEBUG
    #define UPDATE_TIMING_STAT(stat) update_timing_stat(stat, __func__)
#else
    #define UPDATE_TIMING_STAT(stat) ((void) 0)
#endif

#endif