#ifndef TOS_LOG_DEBUG_MEMORY_C
#define TOS_LOG_DEBUG_MEMORY_C

#include "../stdlib/Types.h"
#include "Debug.h"
#include "DebugMemory.h"
#include "Log.h"
#include "TimingStat.h"
#include "../utils/StringUtils.h"

global_persist DebugContainer* debug_container = NULL;

#if _WIN32
    #include <windows.h>
    void setup_performance_count() {
        LARGE_INTEGER perf_counter;
        QueryPerformanceFrequency(&perf_counter);
        debug_container->performance_count_frequency = perf_counter.QuadPart;
    }
#endif

// IMPORTANT: This function should only be called when you actually use this data
//      e.g. log to display or file
inline
void update_timing_stat(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    debug_container->perf_stats[stat].function = function;
    debug_container->perf_stats[stat].delta_tick = new_tick_count - debug_container->perf_stats[stat].old_tick_count;
    debug_container->perf_stats[stat].delta_time = (double) debug_container->perf_stats[stat].delta_tick / (double) debug_container->performance_count_frequency;
    debug_container->perf_stats[stat].old_tick_count = new_tick_count;
}

inline
void update_timing_stat_start(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    debug_container->perf_stats[stat].old_tick_count = new_tick_count;
}

inline
void update_timing_stat_end(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    debug_container->perf_stats[stat].function = function;
    debug_container->perf_stats[stat].delta_tick = new_tick_count - debug_container->perf_stats[stat].old_tick_count;
    debug_container->perf_stats[stat].delta_time = (double) debug_container->perf_stats[stat].delta_tick / (double) debug_container->performance_count_frequency;
    debug_container->perf_stats[stat].old_tick_count = new_tick_count;
}

inline
void update_timing_stat_end_continued(uint32 stat, const char* function)
{
    uint64 new_tick_count = __rdtsc();

    debug_container->perf_stats[stat].function = function;
    debug_container->perf_stats[stat].delta_tick = debug_container->perf_stats[stat].delta_tick
        + new_tick_count - debug_container->perf_stats[stat].old_tick_count;
    debug_container->perf_stats[stat].delta_time = debug_container->perf_stats[stat].delta_time
        + (double) debug_container->perf_stats[stat].delta_tick / (double) debug_container->performance_count_frequency;
    debug_container->perf_stats[stat].old_tick_count = new_tick_count;
}

inline
void update_timing_stat_reset(uint32 stat)
{
    debug_container->perf_stats[stat].function = NULL;
}

inline
void reset_counter(int32 id)
{
    debug_container->counter[id] = 0;
}

inline
void log_increment(int32 id, int32 by = 1)
{
    debug_container->counter[id] += by;
}

inline
void log_counter(int32 id, int32 value)
{
    debug_container->counter[id] = value;
}

// @todo don't use a pointer to this should be in a global together with other logging data (see Log.h)
inline
DebugMemory* debug_memory_find(uint64 start)
{
    for (int32 i = 0; i < debug_container->dmc.memory_size; ++i) {
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

    if (debug_container->dmc.memory_size <= debug_container->dmc.memory_element_idx) {
        DebugMemory* old = debug_container->dmc.memory_stats;

        debug_container->dmc.memory_size += 3;
        debug_container->dmc.memory_stats = (DebugMemory *) calloc(debug_container->dmc.memory_size, sizeof(DebugMemory));

        if (old) {
            memcpy(debug_container->dmc.memory_stats, old, (debug_container->dmc.memory_size - 3) * sizeof(DebugMemory));
            free(old);
        }
    }

    debug_container->dmc.memory_stats[debug_container->dmc.memory_element_idx].start = start;
    debug_container->dmc.memory_stats[debug_container->dmc.memory_element_idx].size = size;
    debug_container->dmc.memory_stats[debug_container->dmc.memory_element_idx].usage = 0;

    ++debug_container->dmc.memory_element_idx;
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

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = type;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();
    mem->last_action[mem->action_idx].function_name = function;

    ++mem->action_idx;
    mem->usage += size * type;
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

    if (mem->reserve_action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->reserve_action_idx = 0;
    }

    mem->reserve_action[mem->reserve_action_idx].type = type;
    mem->reserve_action[mem->reserve_action_idx].start = start - mem->start;
    mem->reserve_action[mem->reserve_action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->reserve_action[mem->reserve_action_idx].time = __rdtsc();
    mem->reserve_action[mem->reserve_action_idx].function_name = function;

    ++mem->reserve_action_idx;
}

inline
void debug_memory_reset()
{
    if (!debug_container) {
        return;
    }

    uint64 time = __rdtsc() - 1000000000;

    for (int32 i = 0; i < debug_container->dmc.memory_element_idx; ++i) {
        for (int32 j = 0; j < DEBUG_MEMORY_RANGE_MAX; ++j) {
            if (debug_container->dmc.memory_stats[i].last_action[j].time < time) {
                memset(&debug_container->dmc.memory_stats[i].last_action[j], 0, sizeof(DebugMemoryRange));
            }
        }
    }
}

byte* log_get_memory(uint64 size, byte aligned = 1, bool zeroed = false)
{
    if (!debug_container) {
        return 0;
    }

    ASSERT_SIMPLE(size <= debug_container->log_memory.size);

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) debug_container->log_memory.memory;
        debug_container->log_memory.pos += (aligned - ((address + debug_container->log_memory.pos) & (aligned - 1))) % aligned;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (debug_container->log_memory.pos + size > debug_container->log_memory.size) {
        debug_container->log_memory.pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) debug_container->log_memory.memory;
            debug_container->log_memory.pos += (aligned - ((address + debug_container->log_memory.pos) & (aligned - 1))) % aligned;
        }
    }

    byte* offset = (byte *) (debug_container->log_memory.memory + debug_container->log_memory.pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    debug_container->log_memory.pos += size;

    return offset;
}

#ifdef _WIN32
    void log_to_file()
    {
        // we don't log an empty log pool
        if (!debug_container || debug_container->log_memory.pos == 0 || !debug_container->log_fp || debug_container->log_fp == INVALID_HANDLE_VALUE) {
            return;
        }

        DWORD written;
        if (!WriteFile(debug_container->log_fp, (char *) debug_container->log_memory.memory, (uint32) debug_container->log_memory.pos - 1, &written, NULL)) {
            CloseHandle(debug_container->log_fp);
        }

        memset(debug_container->log_memory.memory, 0, debug_container->log_memory.size);

        // reset log position to start of memory pool
        debug_container->log_memory.pos = 0;
        debug_container->log_memory.start = 0;
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
        }

        if (save || debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
            log_to_file();
        }

        ASSERT_SIMPLE(false);
    }
#endif

#endif