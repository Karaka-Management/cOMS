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

struct TimingStat {
    const char* function;
    uint64 old_tick_count;
    uint64 delta_tick;
    double delta_time;
};

// Sometimes we want to only do logging in debug mode.
// In such cases use the following macro.
#if DEBUG || INTERNAL
    void update_timing_stat(uint32, const char*);
    #define UPDATE_TIMING_STAT(stat) update_timing_stat(stat, __func__)

    // These are only needed if we need to delay the overwrite by 1 frame (e.g. ui update)
    void update_timing_stat_start(uint32, const char*);
    #define UPDATE_TIMING_STAT_START(stat) update_timing_stat_start(stat, __func__)
    #define UPDATE_TIMING_STAT_CONTINUE(stat) update_timing_stat_start(stat, __func__)

    void update_timing_stat_end(uint32, const char*);
    #define UPDATE_TIMING_STAT_END(stat) update_timing_stat_end(stat, __func__)

    void update_timing_stat_end_continued(uint32, const char*);
    #define UPDATE_TIMING_STAT_END_CONTINUED(stat) update_timing_stat_end_continued(stat, __func__)

    void update_timing_stat_reset(uint32);
    #define UPDATE_TIMING_STAT_RESET(stat) update_timing_stat_reset(stat)
#else
    #define UPDATE_TIMING_STAT(stat) ((void) 0)
    #define UPDATE_TIMING_STAT_START(stat) ((void) 0)
    #define UPDATE_TIMING_STAT_END(stat) ((void) 0)
    #define UPDATE_TIMING_STAT_CONTINUE(stat) ((void) 0)
    #define UPDATE_TIMING_STAT_END_CONTINUED(stat) ((void) 0)
    #define UPDATE_TIMING_STAT_RESET(stat) ((void) 0)
#endif

#endif