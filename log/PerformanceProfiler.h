/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_PERFORMANCE_PROFILER_H
#define TOS_LOG_PERFORMANCE_PROFILER_H

#include "../stdlib/Types.h"
#include "../platform/win32/TimeUtils.h"
#include "../architecture/Intrinsics.h"
#include "../compiler/CompilerUtils.h"

struct PerformanceProfileResult {
    const char* name;
    atomic_64 int64 total_time;
    atomic_64 int64 total_cycle;
    atomic_64 int64 self_time;
    atomic_64 int64 self_cycle;

    // Required for manual start/stop otherwise we would have to use one of the existing values above,
    // which corrupts them for rendering
    atomic_64 int64 tmp_time;
    atomic_64 int64 tmp_cycle;
    PerformanceProfileResult* parent;
};
static PerformanceProfileResult* _perf_stats = NULL;

struct PerformanceProfiler;
static PerformanceProfiler** _perf_current_scope = NULL; // Used when sharing profiler across dlls and threads (threads unlikely)
static PerformanceProfiler* _perf_current_scope_internal; // Used when in dll or thread and no shared pointer found
struct PerformanceProfiler {
    const char* name;
    int32 id;

    int64 start_time;
    int64 total_time;
    int64 self_time;

    int64 start_cycle;
    int64 total_cycle;
    int64 self_cycle;

    PerformanceProfiler* parent;

    PerformanceProfiler(int32 id, const char* scope_name) : id(id) {
        name = scope_name;

        start_time = time_mu();
        start_cycle = intrin_timestamp_counter();

        total_time = 0;
        total_cycle = 0;

        self_time = 0;
        self_cycle = 0;

        if (_perf_current_scope) {
            parent = *_perf_current_scope;
            *_perf_current_scope = this;
        } else {
            parent = _perf_current_scope_internal;
            _perf_current_scope_internal = this;
        }
    }

    ~PerformanceProfiler() {
        uint64 end_time = time_mu();
        uint64 end_cycle = intrin_timestamp_counter();

        total_time = OMS_MAX(end_time - start_time, 0);
        total_cycle = OMS_MAX(end_cycle - start_cycle, 0);

        self_time += total_time;
        self_cycle += total_cycle;

        if (parent) {
            parent->self_time -= total_time;
            parent->self_cycle -= total_cycle;
        }

        // Store result
        PerformanceProfileResult* perf = &_perf_stats[id];
        perf->name = name;
        perf->total_time = total_time;
        perf->total_cycle = total_cycle;
        perf->self_time = self_time;
        perf->self_cycle = self_cycle;
        // @todo create reference to parent result

        if (_perf_current_scope) {
            *_perf_current_scope = parent;
        } else {
            _perf_current_scope_internal = parent;
        }
    }
};

void performance_profiler_reset(uint32 id)
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->total_time = 0;
    perf->total_cycle = 0;
    perf->self_time = 0;
    perf->self_cycle = 0;
}

void performance_profiler_start(uint32 id, const char* name)
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->name = name;

    perf->tmp_time = time_mu();
    perf->tmp_cycle = intrin_timestamp_counter();
}

void performance_profiler_end(uint32 id)
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->total_time = time_mu() - perf->tmp_time;
    perf->total_cycle = intrin_timestamp_counter() - perf->tmp_cycle;
    perf->self_time = perf->total_time;
    perf->self_cycle = perf->self_cycle;
}

#if (!DEBUG && !INTERNAL) || RELEASE
    #define PROFILE_SCOPE(id, name) ((void) 0)
    #define PROFILE(id) ((void) 0)

    #define PROFILE_START(id, name) ((void) 0)
    #define PROFILE_END(id) ((void) 0)

    #define PROFILE_RESET(id) ((void) 0)
#else
    #define PROFILE_SCOPE(id, name) PerformanceProfiler __profile_scope_##id(id, name)

    // Only this function can properly handle self-time calculation
    // Use this whenever you want to profile an entire function
    #define PROFILE(id) PROFILE_SCOPE(id, __func__)

    #define PROFILE_START(id, name) performance_profiler_start(id, name)
    #define PROFILE_END(id) performance_profiler_end(id)

    #define PROFILE_RESET(id) performance_profiler_reset((id))
#endif

#endif