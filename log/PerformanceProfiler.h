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
#include "../thread/Spinlock.cpp"
#include "../thread/Atomic.h"
#include "../system/Allocator.h"
#include "../hash/GeneralHash.h"
#include "../architecture/Intrinsics.h"
#include "../compiler/CompilerUtils.h"
#include "Log.h"

#ifndef PERFORMANCE_PROFILE_STATS
    #define PERFORMANCE_PROFILE_STATS 1
    enum TimingStats {
        PROFILE_TEMP, // used for quick test debugging, not for permanent use

        PROFILE_FILE_UTILS,
        PROFILE_BUFFER_ALLOC,
        PROFILE_CHUNK_ALLOC,
        PROFILE_RING_ALLOC,
        PROFILE_CMD_ITERATE,
        PROFILE_CMD_FONT_LOAD_SYNC,
        PROFILE_CMD_SHADER_LOAD_SYNC,
        PROFILE_CMD_LAYOUT_LOAD_SYNC,
        PROFILE_CMD_THEME_LOAD_SYNC,
        PROFILE_CMD_UI_LOAD_SYNC,
        PROFILE_LAYOUT_FROM_DATA,
        PROFILE_LAYOUT_FROM_THEME,
        PROFILE_THEME_FROM_THEME,
        PROFILE_AUDIO_BUFFER_FILLABLE,
        PROFILE_AUDIO_PLAY_BUFFER,
        PROFILE_AUDIO_MIXER_MIX,
        PROFILE_ASSET_ARCHIVE_LOAD,
        PROFILE_ASSET_ARCHIVE_ASSET_LOAD,
        PROFILE_VERTEX_RECT_CREATE,
        PROFILE_VERTEX_TEXT_CREATE,

        PROFILE_SIZE,
    };
#endif

struct PerformanceProfileResult {
    atomic_64 const char* name;

    atomic_64 int64 total_cycle;
    atomic_64 int64 self_cycle;

    atomic_32 uint32 counter;
    uint32 parent;
};
static PerformanceProfileResult* _perf_stats = NULL;
static int32* _perf_active = NULL;

struct PerformanceProfiler;
static thread_local PerformanceProfiler** _perf_current_scope = NULL; // Used when sharing profiler across dlls and threads (threads unlikely)
static thread_local PerformanceProfiler* _perf_current_scope_internal; // Used when in dll or thread and no shared pointer found
struct PerformanceProfiler {
    bool is_active;

    const char* name;
    const char* info_msg;
    int32 id;

    int64 start_cycle;
    int64 total_cycle;
    int64 self_cycle;

    PerformanceProfiler* parent;

    bool auto_log;
    bool is_stateless;

    // @question Do we want to make the self cost represent calls * "self_time/cycle"
    // Stateless allows to ONLY output to log instead of storing the performance data in an array
    PerformanceProfiler(
        int32 id, const char* scope_name, const char* info = NULL,
        bool stateless = false, bool should_log = false
    ) {
        if (!_perf_active || !*_perf_active) {
            this->is_active = false;

            return;
        }

        this->id = id;
        ++_perf_stats[id].counter;

        this->name = scope_name;
        this->info_msg = info;
        this->is_stateless = stateless;
        this->auto_log = stateless || should_log;

        this->start_cycle = intrin_timestamp_counter();
        this->total_cycle = 0;
        this->self_cycle = 0;

        if (this->is_stateless) {
            this->parent = NULL;
        } else {
            if (_perf_current_scope) {
                this->parent = *_perf_current_scope;
                *_perf_current_scope = this;
            } else {
                this->parent = _perf_current_scope_internal;
                _perf_current_scope_internal = this;
            }
        }
    }

    ~PerformanceProfiler() {
        if (!this->is_active) {
            return;
        }

        uint64 end_cycle = intrin_timestamp_counter();
        this->total_cycle = OMS_MAX(end_cycle - start_cycle, 0);
        this->self_cycle += total_cycle;

        // Store result
        PerformanceProfileResult temp_perf = {};
        PerformanceProfileResult* perf = this->is_stateless ? &temp_perf : &_perf_stats[this->id];

        perf->name = this->name;
        perf->total_cycle = this->total_cycle;
        perf->self_cycle = this->self_cycle;

        if (!this->is_stateless) {
            if (this->parent) {
                this->parent->self_cycle -= this->total_cycle;
                perf->parent = this->parent->id;
            }

            if (_perf_current_scope) {
                *_perf_current_scope = this->parent;
            } else {
                _perf_current_scope_internal = this->parent;
            }
        }

        if (this->auto_log) {
            if (this->info_msg && this->info_msg[0]) {
                LOG_FORMAT_2(
                    "%s (%s): %l cycles",
                    {
                        {LOG_DATA_CHAR_STR, (void *) perf->name},
                        {LOG_DATA_CHAR_STR, (void *) this->info_msg},
                        {LOG_DATA_INT64, (void *) &perf->total_cycle},
                    }
                );
            } else {
                LOG_FORMAT_2(
                    "%s: %l cycles",
                    {
                        {LOG_DATA_CHAR_STR, (void *) perf->name},
                        {LOG_DATA_INT64, (void *) &perf->total_cycle},
                    }
                );
            }
        }
    }
};

inline
void performance_profiler_reset(int32 id) noexcept
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->total_cycle = 0;
    perf->self_cycle = 0;
    perf->parent = NULL;
}

inline
void performance_profiler_start(int32 id, const char* name) noexcept
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->name = name;
    perf->self_cycle = -((int64) intrin_timestamp_counter());
}

inline
void performance_profiler_end(int32 id) noexcept
{
    PerformanceProfileResult* perf = &_perf_stats[id];
    perf->total_cycle = intrin_timestamp_counter() + perf->self_cycle;
    perf->self_cycle = perf->total_cycle;
}

// @question Do we want different levels of PROFILE_VERBOSE and PROFILE_STATELESS same as in Log.h
// This would allow us to go ham in a lot of functions (e.g. file reading)

#if LOG_LEVEL == 4
    // Only these function can properly handle self-time calculation
    // Use these whenever you want to profile an entire function
    #define PROFILE(id) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__)
    #define PROFILE_VERBOSE(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), false, true)
    #define PROFILE_STATELESS(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), true, true)

    #define PROFILE_START(id, name) if(_perf_active && *_perf_active) performance_profiler_start((id), (name))
    #define PROFILE_END(id) if(_perf_active && *_perf_active) performance_profiler_end((id))
    #define PROFILE_SCOPE(id, name) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), (name))
    #define PROFILE_RESET(id) if(_perf_active && *_perf_active) performance_profiler_reset((id))
#elif LOG_LEVEL == 3
    // Only these function can properly handle self-time calculation
    // Use these whenever you want to profile an entire function
    #define PROFILE(id) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__)
    #define PROFILE_VERBOSE(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), false, true)
    #define PROFILE_STATELESS(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), true, true)

    #define PROFILE_START(id, name) if(_perf_active && *_perf_active) performance_profiler_start((id), (name))
    #define PROFILE_END(id) if(_perf_active && *_perf_active) performance_profiler_end((id))
    #define PROFILE_SCOPE(id, name) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), (name))
    #define PROFILE_RESET(id) if(_perf_active && *_perf_active) performance_profiler_reset((id))
#elif LOG_LEVEL == 2
    // Only these function can properly handle self-time calculation
    // Use these whenever you want to profile an entire function
    #define PROFILE(id) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__)
    #define PROFILE_VERBOSE(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), false, true)
    #define PROFILE_STATELESS(id, info) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), __func__, (info), true, true)

    #define PROFILE_START(id, name) if(_perf_active && *_perf_active) performance_profiler_start((id), (name))
    #define PROFILE_END(id) if(_perf_active && *_perf_active) performance_profiler_end((id))
    #define PROFILE_SCOPE(id, name) PerformanceProfiler __profile_scope_##__func__##_##__LINE__((id), (name))
    #define PROFILE_RESET(id) if(_perf_active && *_perf_active) performance_profiler_reset((id))
#elif LOG_LEVEL == 1
    #define PROFILE(id) ((void) 0)
    #define PROFILE_VERBOSE(name) ((void) 0)
    #define PROFILE_STATELESS(id, info) ((void) 0)

    #define PROFILE_START(id, name) ((void) 0)
    #define PROFILE_END(id) ((void) 0)
    #define PROFILE_SCOPE(id, name) ((void) 0)
    #define PROFILE_RESET(id) ((void) 0)
#elif LOG_LEVEL == 0
    #define PROFILE(id) ((void) 0)
    #define PROFILE_VERBOSE(name) ((void) 0)
    #define PROFILE_STATELESS() ((void) 0)

    #define PROFILE_START(id, name) ((void) 0)
    #define PROFILE_END(id) ((void) 0)
    #define PROFILE_SCOPE(id, name) ((void) 0)
    #define PROFILE_RESET(id) ((void) 0)
#endif

#endif