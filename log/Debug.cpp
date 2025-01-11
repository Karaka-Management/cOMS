#ifndef TOS_LOG_DEBUG_MEMORY_C
#define TOS_LOG_DEBUG_MEMORY_C

#include "../stdlib/Types.h"
#include "Debug.h"
#include "DebugMemory.h"
#include "Log.h"
#include "TimingStat.h"
#include "../utils/StringUtils.h"
#include "../utils/TestUtils.h"
#include "../thread/Atomic.h"

// Required for rdtsc();
#if _WIN32
    #include <intrin.h>
#else
    #include <x86intrin.h>
#endif

global_persist DebugContainer* debug_container = NULL;

// WARNING: Spinlock uses TimeUtils which uses performance counter, which is part of DebugContainer
// @todo The explanation above is insane. We did this so we only have to set the performance counter once but it is biting us now
#include "../thread/Spinlock.cpp"

#if _WIN32
    #include <windows.h>
    void setup_performance_count() {
        if (!debug_container) {
            return;
        }

        LARGE_INTEGER perf_counter;
        QueryPerformanceFrequency(&perf_counter);
        debug_container->performance_count_frequency = perf_counter.QuadPart;
    }
#elif __linux__
    void setup_performance_count() {
        if (!debug_container) {
            return;
        }

        FILE* fp = fopen("/proc/cpuinfo", "r");
        if (!fp) {
            return;
        }

        char line[256];
        uint64 cpu_freq = 0;
        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "cpu MHz%*[^0-9]%ld", &cpu_freq) == 1) {
                break;
            }
        }

        fclose(fp);

        debug_container->performance_count_frequency = cpu_freq == 0 ? 1 : cpu_freq * 1000000;
    }
#endif

void log_to_file()
{
    // we don't log an empty log pool
    if (!debug_container || debug_container->log_memory.pos == 0 || !debug_container->log_fp) {
        return;
    }

    #if _WIN32
        DWORD written;
        WriteFile(
            debug_container->log_fp,
            (char *) debug_container->log_memory.memory,
            (uint32) debug_container->log_memory.pos,
            &written,
            NULL
        );
    #else
        if (debug_container->log_fp < 0) {
            return;
        }

        write(
            debug_container->log_fp,
            (char *) debug_container->log_memory.memory,
            (uint32) debug_container->log_memory.pos
        );
    #endif
}

// IMPORTANT: This function should only be called when you actually use this data
//      e.g. log to display or file
inline
void update_timing_stat(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    TimingStat* timing_stat = &debug_container->perf_stats[stat];

    spinlock_start(&debug_container->perf_stats_spinlock);
    timing_stat->function = function;
    timing_stat->delta_tick = (uint32) (new_tick_count - timing_stat->old_tick_count);
    timing_stat->delta_time = (f64) timing_stat->delta_tick / (f64) debug_container->performance_count_frequency;
    timing_stat->old_tick_count = new_tick_count;
    spinlock_end(&debug_container->perf_stats_spinlock);
}

inline
void update_timing_stat_start(uint32 stat, const char*)
{
    spinlock_start(&debug_container->perf_stats_spinlock);
    debug_container->perf_stats[stat].old_tick_count = __rdtsc();
    spinlock_end(&debug_container->perf_stats_spinlock);
}

inline
void update_timing_stat_end(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    TimingStat* timing_stat = &debug_container->perf_stats[stat];

    spinlock_start(&debug_container->perf_stats_spinlock);
    timing_stat->function = function;
    timing_stat->delta_tick = (uint32) (new_tick_count - timing_stat->old_tick_count);
    timing_stat->delta_time = (f64) timing_stat->delta_tick / (f64) debug_container->performance_count_frequency;
    timing_stat->old_tick_count = new_tick_count;
    spinlock_end(&debug_container->perf_stats_spinlock);
}

inline
void update_timing_stat_end_continued(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    TimingStat* timing_stat = &debug_container->perf_stats[stat];

    spinlock_start(&debug_container->perf_stats_spinlock);
    timing_stat->function = function;
    timing_stat->delta_tick = (uint32) ((uint32) (new_tick_count - timing_stat->old_tick_count) + timing_stat->delta_tick);
    timing_stat->delta_time = timing_stat->delta_time + (f64) timing_stat->delta_tick / (f64) debug_container->performance_count_frequency;
    timing_stat->old_tick_count = new_tick_count;
    spinlock_end(&debug_container->perf_stats_spinlock);
}

inline
void update_timing_stat_reset(uint32 stat)
{
    TimingStat* timing_stat = &debug_container->perf_stats[stat];

    spinlock_start(&debug_container->perf_stats_spinlock);
    timing_stat->function = NULL;
    timing_stat->delta_tick = 0;
    timing_stat->delta_time = 0;
    spinlock_end(&debug_container->perf_stats_spinlock);
}

inline
void reset_counter(int32 id)
{
    atomic_set_acquire(&debug_container->counter[id], 0);
}

inline
void log_increment(int32 id, int64 by = 1)
{
    atomic_add_acquire(&debug_container->counter[id], by);
}

inline
void log_counter(int32 id, int64 value)
{
    atomic_set_acquire(&debug_container->counter[id], value);
}

inline
DebugMemory* debug_memory_find(uintptr_t start)
{
    for (uint64 i = 0; i < debug_container->dmc.memory_size; ++i) {
        if (debug_container->dmc.memory_stats[i].start <= start
            && debug_container->dmc.memory_stats[i].start + debug_container->dmc.memory_stats[i].size >= start
        ) {
            return &debug_container->dmc.memory_stats[i];
        }
    }

    return NULL;
}

void debug_memory_init(uintptr_t start, uint64 size)
{
    if (!start || !debug_container) {
        return;
    }

    const DebugMemory* mem = debug_memory_find(start);
    if (mem) {
        return;
    }

    DebugMemoryContainer* dmc = &debug_container->dmc;
    if (dmc->memory_size <= dmc->memory_element_idx) {
        DebugMemory* old = dmc->memory_stats;

        dmc->memory_size += 3;
        dmc->memory_stats = (DebugMemory *) calloc(dmc->memory_size, sizeof(DebugMemory));

        if (old) {
            memcpy(dmc->memory_stats, old, (dmc->memory_size - 3) * sizeof(DebugMemory));
            free(old);
        }
    }

    DebugMemory* debug_mem = &dmc->memory_stats[dmc->memory_element_idx];
    debug_mem->start = start;
    debug_mem->size = size;
    debug_mem->usage = 0;

    ++dmc->memory_element_idx;
}

void debug_memory_log(uintptr_t start, uint64 size, int32 type, const char* function)
{
    if (!start || !debug_container) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start);
    if (!mem) {
        return;
    }

    uint64 idx = atomic_fetch_add_relaxed(&mem->action_idx, 1);
    if (idx >= ARRAY_COUNT(mem->last_action)) {
        atomic_set_acquire(&mem->action_idx, 1);
        idx %= ARRAY_COUNT(mem->last_action);
    }

    DebugMemoryRange* dmr = &mem->last_action[idx];
    dmr->type = type;
    dmr->start = start - mem->start;
    dmr->size = size;

    dmr->time = __rdtsc();
    dmr->function_name = function;

    if (type < 0 && mem->usage < size * -type) {
        mem->usage = 0;
    } else {
        mem->usage += size * type;
    }
}

void debug_memory_reserve(uintptr_t start, uint64 size, int32 type, const char* function)
{
    if (!start || !debug_container) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start);
    if (!mem) {
        return;
    }

    uint64 idx = atomic_fetch_add_relaxed(&mem->reserve_action_idx, 1);
    if (idx >= ARRAY_COUNT(mem->reserve_action)) {
        atomic_set_acquire(&mem->reserve_action_idx, 1);
        idx %= ARRAY_COUNT(mem->reserve_action);
    }

    DebugMemoryRange* dmr = &mem->reserve_action[idx];
    dmr->type = type;
    dmr->start = start - mem->start;
    dmr->size = size;

    dmr->time = __rdtsc();
    dmr->function_name = function;
}

// undo reserve
void debug_memory_free(uintptr_t start, uint64 size)
{
    if (!start || !debug_container) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start);
    if (!mem) {
        return;
    }

    for (int32 i = 0; i < ARRAY_COUNT(mem->reserve_action); ++i) {
        DebugMemoryRange* dmr = &mem->reserve_action[i];
        if (dmr->start == start - mem->start) {
            dmr->size = 0;
            return;
        }
    }
}

// @bug This probably requires thread safety
inline
void debug_memory_reset()
{
    if (!debug_container) {
        return;
    }

    // We remove debug information that are "older" than 1GHz
    uint64 time = __rdtsc() - 1 * GHZ;

    for (uint64 i = 0; i < debug_container->dmc.memory_element_idx; ++i) {
        for (int32 j = 0; j < DEBUG_MEMORY_RANGE_MAX; ++j) {
            if (debug_container->dmc.memory_stats[i].last_action[j].time < time) {
                memset(&debug_container->dmc.memory_stats[i].last_action[j], 0, sizeof(DebugMemoryRange));
            }
        }
    }
}

// @bug This probably requires thread safety
byte* log_get_memory()
{
    LogMemory* log_mem = &debug_container->log_memory;
    if (log_mem->pos + MAX_LOG_LENGTH > log_mem->size) {
        log_mem->pos = 0;
    }

    byte* offset = (byte *) (log_mem->memory + log_mem->pos);
    memset((void *) offset, 0, MAX_LOG_LENGTH);

    log_mem->pos += MAX_LOG_LENGTH;

    return offset;
}

void log(const char* str, bool should_log, const char* file, const char* function, int32 line)
{
    if (!should_log || !debug_container) {
        return;
    }

    int64 len = strlen(str);
    while (len > 0) {
        LogMessage* msg = (LogMessage *) log_get_memory();

        // Fill file
        msg->file = file;
        msg->function = function;
        msg->line = line;
        msg->message = (char *) (msg + 1);

        int32 message_length = (int32) OMS_MIN(MAX_LOG_LENGTH - sizeof(LogMessage) - 1, len);

        memcpy(msg->message, str, message_length);
        msg->message[message_length] = '\0';
        str += message_length;
        len -= MAX_LOG_LENGTH - sizeof(LogMessage);

        if (debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
            log_to_file();
            debug_container->log_memory.pos = 0;
        }
    }
}

void log(const char* format, LogDataArray data, bool should_log, const char* file, const char* function, int32 line)
{
    ASSERT_SIMPLE(strlen(format) + strlen(file) + strlen(function) + 50 < MAX_LOG_LENGTH);

    if (!should_log || !debug_container) {
        return;
    }

    if (data.data[0].type == LOG_DATA_VOID) {
        log(format, should_log, file, function, line);
        return;
    }

    LogMessage* msg = (LogMessage *) log_get_memory();
    msg->file = file;
    msg->function = function;
    msg->line = line;
    msg->message = (char *) (msg + 1);

    char temp_format[MAX_LOG_LENGTH];
    str_copy_short(msg->message, format);

    for (int32 i = 0; i < LOG_DATA_ARRAY; ++i) {
        if (data.data[i].type == LOG_DATA_VOID) {
            break;
        }

        str_copy_short(temp_format, msg->message);

        switch (data.data[i].type) {
            case LOG_DATA_BYTE: {
                sprintf_fast_iter(msg->message, temp_format, (int32) *((byte *) data.data[i].value));
            } break;
            case LOG_DATA_INT32: {
                sprintf_fast_iter(msg->message, temp_format, *((int32 *) data.data[i].value));
            } break;
            case LOG_DATA_UINT32: {
                sprintf_fast_iter(msg->message, temp_format, *((uint32 *) data.data[i].value));
            } break;
            case LOG_DATA_INT64: {
                sprintf_fast_iter(msg->message, temp_format, *((int64 *) data.data[i].value));
            } break;
            case LOG_DATA_UINT64: {
                sprintf_fast_iter(msg->message, temp_format, *((uint64 *) data.data[i].value));
            } break;
            case LOG_DATA_CHAR: {
                sprintf_fast_iter(msg->message, temp_format, *((char *) data.data[i].value));
            } break;
            case LOG_DATA_CHAR_STR: {
                sprintf_fast_iter(msg->message, temp_format, (const char *) data.data[i].value);
            } break;
            case LOG_DATA_FLOAT32: {
                sprintf_fast_iter(msg->message, temp_format, *((f32 *) data.data[i].value));
            } break;
            case LOG_DATA_FLOAT64: {
                sprintf_fast_iter(msg->message, temp_format, *((f64 *) data.data[i].value));
            } break;
            default: {
                UNREACHABLE();
            }
        }
    }

    if (debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
        log_to_file();
        debug_container->log_memory.pos = 0;
    }
}

#endif