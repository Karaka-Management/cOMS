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
#include "../platform/win32/UtilsWin32.h"
#include "../memory/RingMemory.h"

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

void log_to_file(RingMemory* logs, HANDLE fp)
{
    // we don't log an empty log pool
    if (logs->pos == 0 || !fp) {
        return;
    }

    file_append(fp, (char *) logs->memory, logs->pos - 1);
    memset(logs->memory, 0, logs->size);

    // reset log position to start of memory pool
    logs->pos = 0;
    logs->start = 0;
}

void log(RingMemory* logs, const char* str, HANDLE fp = NULL, bool should_log = true, bool save = false)
{
    if (!should_log) {
        return;
    }

    size_t length = strlen(str);
    ASSERT_SIMPLE(length < MAX_LOG_LENGTH);

    char* temp = (char *) ring_get_memory(logs, length + 1);
    strcpy(temp, str);
    temp[length] = '\0';

    if (fp != NULL && (save || logs->size - logs->pos < MAX_LOG_LENGTH)) {
        log_to_file(logs, fp);
    }
}

void log(RingMemory* logs, const char* format, LogDataType data_type, void* data, HANDLE fp = NULL, bool should_log = true, bool save = false)
{
    if (!should_log) {
        return;
    }

    if (data_type == LOG_DATA_VOID) {
        log(logs, format, fp, true);
    }

    char* temp = (char *) ring_get_memory(logs, MAX_LOG_LENGTH);

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