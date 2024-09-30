#ifndef TOS_LOG_DEBUG_MEMORY_C
#define TOS_LOG_DEBUG_MEMORY_C

#include "Debug.h"
#include "DebugMemory.h"
#include "Log.h"
#include "TimingStat.h"

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

// @todo don't use a pointer to this should be in a global together with other logging data (see Log.h)
inline
DebugMemory* debug_memory_find(uint64 start, uint64 size)
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
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
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

void debug_memory_write(uint64 start, uint64 size)
{
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = 1;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
    mem->usage += size;
}

void debug_memory_read(uint64 start, uint64 size)
{
    if (!start) {
        return;
    }

    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = 0;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
}

void debug_memory_delete(uint64 start, uint64 size)
{
    DebugMemory* mem = debug_memory_find(start, size);
    if (!mem) {
        return;
    }

    if (mem->action_idx == DEBUG_MEMORY_RANGE_MAX) {
        mem->action_idx = 0;
    }

    mem->last_action[mem->action_idx].type = -1;
    mem->last_action[mem->action_idx].start = start - mem->start;
    mem->last_action[mem->action_idx].size = size;

    // @question consider to use other time_ms() since __rdtsc is variable (boost, power saving)
    mem->last_action[mem->action_idx].time = __rdtsc();

    ++mem->action_idx;
    mem->usage -= size;
}

inline
void debug_memory_reset()
{
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
    ASSERT_SIMPLE(size <= debug_container->log_memory.size);

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) debug_container->log_memory.memory;
        int64 adjustment = (aligned - ((address + debug_container->log_memory.pos) & (aligned - 1))) % aligned;

        debug_container->log_memory.pos += adjustment;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (debug_container->log_memory.pos + size > debug_container->log_memory.size) {
        debug_container->log_memory.pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) debug_container->log_memory.memory;
            int64 adjustment = (aligned - ((address + debug_container->log_memory.pos) & (aligned - 1))) % aligned;

            debug_container->log_memory.pos += adjustment;
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
        if (debug_container->log_memory.pos == 0 || !debug_container->log_fp || debug_container->log_fp == INVALID_HANDLE_VALUE) {
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
        if (!should_log) {
            return;
        }

        size_t length = strlen(str);
        ASSERT_SIMPLE(length < MAX_LOG_LENGTH);

        char* temp = (char *) log_get_memory(length + 1);
        strcpy(temp, str);
        temp[length] = '\0';

        if (save || debug_container->log_memory.size - debug_container->log_memory.pos < MAX_LOG_LENGTH) {
            log_to_file();
        }
    }

    void log(const char* format, LogDataType data_type, void* data, bool should_log, bool save, const char* file, const char* function, int32 line)
    {
        if (!should_log) {
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
    }
#endif

#endif