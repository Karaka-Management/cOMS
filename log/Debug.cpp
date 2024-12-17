#ifndef TOS_LOG_DEBUG_MEMORY_C
#define TOS_LOG_DEBUG_MEMORY_C

#include "../stdlib/Types.h"
#include "Debug.h"
#include "DebugMemory.h"
#include "Log.h"
#include "TimingStat.h"
#include "../utils/StringUtils.h"
#include "../utils/TestUtils.h"
#include "../utils/MathUtils.h"

global_persist DebugContainer* debug_container = NULL;

#if _WIN32
    #include <windows.h>
    #include "../platform/win32/threading/Atomic.h"
    #include "../platform/win32/threading/Spinlock.cpp"
    void setup_performance_count() {
        if (!debug_container) {
            return;
        }

        LARGE_INTEGER perf_counter;
        QueryPerformanceFrequency(&perf_counter);
        debug_container->performance_count_frequency = perf_counter.QuadPart;
    }
#elif __linux__
#include "../platform/linux/threading/Atomic.h"
#include "../platform/linux/threading/Spinlock.cpp"
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
        if (!WriteFile(
            debug_container->log_fp,
            (char *) debug_container->log_memory.memory,
            (uint32) debug_container->log_memory.pos - 1,
            &written,
            NULL
        )) {
            CloseHandle(debug_container->log_fp);
        }
    #else
        if (debug_container->log_fp < 0) {
            return;
        }

        if (!write(
            debug_container->log_fp,
            (char *) debug_container->log_memory.memory,
            (uint32) debug_container->log_memory.pos - 1
        )) {
            close(debug_container->log_fp);
        }
    #endif

    memset(debug_container->log_memory.memory, 0, debug_container->log_memory.size);

    // reset log position to start of memory pool
    debug_container->log_memory.pos = 0;
    debug_container->log_memory.start = 0;
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
    timing_stat->delta_tick = new_tick_count - timing_stat->old_tick_count;
    timing_stat->delta_time = (double) timing_stat->delta_tick / (double) debug_container->performance_count_frequency;
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
    timing_stat->delta_tick = new_tick_count - timing_stat->old_tick_count;
    timing_stat->delta_time = (double) timing_stat->delta_tick / (double) debug_container->performance_count_frequency;
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
    timing_stat->delta_tick = timing_stat->delta_tick + new_tick_count - timing_stat->old_tick_count;
    timing_stat->delta_time = timing_stat->delta_time + (double) timing_stat->delta_tick / (double) debug_container->performance_count_frequency;
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
    atomic_set(&debug_container->counter[id], 0);
}

inline
void log_increment(int32 id, int64 by = 1)
{
    atomic_add(&debug_container->counter[id], by);
}

inline
void log_counter(int32 id, int64 value)
{
    atomic_set(&debug_container->counter[id], value);
}

// @todo don't use a pointer to this should be in a global together with other logging data (see Log.h)
inline
DebugMemory* debug_memory_find(uint64 start)
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

void debug_memory_init(uint64 start, uint64 size)
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

void debug_memory_log(uint64 start, uint64 size, int32 type, const char* function)
{
    if (!start || !debug_container) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start);
    if (!mem) {
        return;
    }

    uint64 idx = atomic_fetch_add(&mem->action_idx, 1);
    if (idx >= ARRAY_COUNT(mem->last_action)) {
        atomic_set(&mem->action_idx, 1);
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

void debug_memory_reserve(uint64 start, uint64 size, int32 type, const char* function)
{
    if (!start || !debug_container) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start);
    if (!mem) {
        return;
    }

    uint64 idx = atomic_fetch_add(&mem->reserve_action_idx, 1);
    if (idx >= ARRAY_COUNT(mem->reserve_action)) {
        atomic_set(&mem->reserve_action_idx, 1);
        idx %= ARRAY_COUNT(mem->last_action);
    }

    DebugMemoryRange* dmr = &mem->reserve_action[idx];
    dmr->type = type;
    dmr->start = start - mem->start;
    dmr->size = size;

    dmr->time = __rdtsc();
    dmr->function_name = function;
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
byte* log_get_memory(uint64 size, byte aligned = 1, bool zeroed = false)
{
    if (!debug_container) {
        return 0;
    }

    LogMemory* log_mem = &debug_container->log_memory;
    ASSERT_SIMPLE(size <= log_mem->size);

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) log_mem->memory;
        log_mem->pos += (aligned - ((address + log_mem->pos) & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (log_mem->pos + size > log_mem->size) {
        log_mem->pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) log_mem->memory;
            log_mem->pos += (aligned - ((address + log_mem->pos) & (aligned - 1))) % aligned;
        }
    }

    byte* offset = (byte *) (log_mem->memory + log_mem->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    log_mem->pos += size;

    return offset;
}

// @todo add file name, function name and function line
void log(const char* str, bool should_log, bool save, const char* file, const char* function, int32 line)
{
    if (!should_log || !debug_container) {
        return;
    }

    size_t str_len = strlen(str);
    size_t file_len = strlen(file);
    size_t function_len = strlen(function);

    char line_str[10];
    int_to_str(line, line_str, '\0');

    size_t line_len = strlen(line_str);

    ASSERT_SIMPLE(str_len + file_len + function_len + line_len + 3 < MAX_LOG_LENGTH);

    char* temp = (char *) log_get_memory(str_len + file_len + function_len + line_len + 3 + 1);
    memcpy(temp, file, file_len);
    temp[file_len] = ';';

    memcpy(&temp[file_len], function, function_len);
    temp[file_len + 1 + function_len] = ';';

    memcpy(&temp[file_len + 1 + function_len], line_str, line_len);
    temp[file_len + 1 + function_len + 1 + line_len] = ';';

    memcpy(&temp[file_len + 1 + function_len + 1 + line_len + 1], str, str_len);
    temp[file_len + 1 + function_len + 1 + line_len + 1 + str_len] = '\0';

    if (save || debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
        log_to_file();
    }

    ASSERT_SIMPLE(false);
}

void log(const char* format, LogDataType data_type, void* data, bool should_log, bool save, const char* file, const char* function, int32 line)
{
    if (!should_log || !debug_container) {
        return;
    }

    if (data_type == LOG_DATA_VOID) {
        log(format, should_log, save, file, function, line);
    }

    char* temp = (char *) log_get_memory(MAX_LOG_LENGTH);

    switch (data_type) {
        case LOG_DATA_INT32: {
            sprintf(temp, format, *((int32 *) data));
        } break;
        case LOG_DATA_UINT32: {
            sprintf(temp, format, *((uint32 *) data));
        } break;
        case LOG_DATA_INT64: {
            sprintf(temp, format, *((int64 *) data));
        } break;
        case LOG_DATA_UINT64: {
            sprintf(temp, format, *((uint64 *) data));
        } break;
        case LOG_DATA_CHAR: {
            sprintf(temp, format, *((char *) data));
        } break;
        case LOG_DATA_CHAR_STR: {
            sprintf(temp, format, *((char *) data));
        } break;
        case LOG_DATA_FLOAT32: {
            sprintf(temp, format, *((f32 *) data));
        } break;
        case LOG_DATA_FLOAT64: {
            sprintf(temp, format, *((f64 *) data));
        } break;
        default: {
            UNREACHABLE();
        }
    }

    if (save || debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
        log_to_file();
    }

    ASSERT_SIMPLE(false);
}


#endif