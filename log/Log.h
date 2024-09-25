/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_LOG_H
#define TOS_PLATFORM_WIN32_LOG_H

#include <stdio.h>
#include <string.h>

#include "../stdlib/Types.h"
#include "../utils/TestUtils.h"
#include "../utils/MathUtils.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#ifndef LOG_LEVEL
    #define LOG_LEVEL 0
#endif

#ifndef MAX_LOG_LENGTH
    #define MAX_LOG_LENGTH 128
#endif

enum LogDataType {
    LOG_DATA_VOID,
    LOG_DATA_INT32,
    LOG_DATA_UINT32,
    LOG_DATA_INT64,
    LOG_DATA_UINT64,
    LOG_DATA_CHAR,
    LOG_DATA_CHAR_STR,
    LOG_DATA_FLOAT32,
    LOG_DATA_FLOAT64
};

struct LogMemory {
    byte* memory;

    uint32 id;
    uint64 size;
    uint64 pos;
    int alignment;

    uint64 start;
    uint64 end;
};

byte* log_get_memory(LogMemory* ring, uint64 size, byte aligned = 1, bool zeroed = false)
{
    ASSERT_SIMPLE(size <= ring->size);

    if (aligned > 1) {
        uintptr_t address = (uintptr_t) ring->memory;
        int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

        ring->pos += adjustment;
    }

    size = ROUND_TO_NEAREST(size, aligned);
    if (ring->pos + size > ring->size) {
        ring->pos = 0;

        if (aligned > 1) {
            uintptr_t address = (uintptr_t) ring->memory;
            int64 adjustment = (aligned - ((address + ring->pos) & (aligned - 1))) % aligned;

            ring->pos += adjustment;
        }
    }

    byte* offset = (byte *) (ring->memory + ring->pos);
    if (zeroed) {
        memset((void *) offset, 0, size);
    }

    ring->pos += size;

    return offset;
}

#ifdef _WIN32
    void log_to_file(LogMemory* logs, HANDLE fp)
    {
        // we don't log an empty log pool
        if (logs->pos == 0 || !fp || fp == INVALID_HANDLE_VALUE) {
            return;
        }

        DWORD written;
        if (!WriteFile(fp, (char *) logs->memory, (uint32) logs->pos - 1, &written, NULL)) {
            CloseHandle(fp);
        }

        memset(logs->memory, 0, logs->size);

        // reset log position to start of memory pool
        logs->pos = 0;
        logs->start = 0;
    }

    void log(LogMemory* logs, const char* str, HANDLE fp = NULL, bool should_log = true, bool save = false)
    {
        if (!should_log) {
            return;
        }

        size_t length = strlen(str);
        ASSERT_SIMPLE(length < MAX_LOG_LENGTH);

        char* temp = (char *) log_get_memory(logs, length + 1);
        strcpy(temp, str);
        temp[length] = '\0';

        if (fp != NULL && (save || logs->size - logs->pos < MAX_LOG_LENGTH)) {
            log_to_file(logs, fp);
        }
    }

    void log(LogMemory* logs, const char* format, LogDataType data_type, void* data, HANDLE fp = NULL, bool should_log = true, bool save = false)
    {
        if (!should_log) {
            return;
        }

        if (data_type == LOG_DATA_VOID) {
            log(logs, format, fp, true);
        }

        char* temp = (char *) log_get_memory(logs, MAX_LOG_LENGTH);

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

        if (fp != NULL && (save || logs->size - logs->pos < MAX_LOG_LENGTH)) {
            log_to_file(logs, fp);
        }
    }
#endif

#if (LOG_LEVEL == 0)
    // Don't perform any logging at log level 0
    #define LOG(logs, str, fp, should_log, save) ((void) 0)
    #define LOG_FORMAT(logs, format, data_type, data, fp, should_log, save) ((void) 0)
    #define LOG_TO_FILE(logs, fp, should_log, save) ((void) 0)
#else
    #define LOG(logs, str, fp, should_log, save) log((logs), (str), (fp), (should_log), (save))
    #define LOG_FORMAT(logs, format, data_type, data, fp, should_log, save) log((logs), (format), (data_type), (data), (fp), (should_log), (save))
    #define LOG_TO_FILE(logs, fp, should_log, save) log_to_file((logs), (fp), (should_log), (save))
#endif

#if DEBUG
    #define DEBUG_LOG(logs, str, fp, should_log, save) log((logs), (str), (fp), (should_log), (save))
    #define DEBUG_LOG_FORMAT(logs, format, data_type, data, fp, should_log, save) log((logs), (format), (data_type), (data), (fp), (should_log), (save))
#else
    #define DEBUG_LOG(logs, str, fp, should_log, save) ((void) 0)
    #define DEBUG_LOG_FORMAT(logs, format, data_type, data, fp, should_log, save) ((void) 0)
#endif

#endif