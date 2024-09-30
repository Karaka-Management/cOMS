/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_H
#define TOS_LOG_H

#include <stdio.h>
#include <string.h>

#include "../stdlib/Types.h"
#include "../utils/TestUtils.h"
#include "../utils/MathUtils.h"
#include "Debug.h"

#ifdef _WIN32
    #include <windows.h>
#endif

#ifndef LOG_LEVEL
    #define LOG_LEVEL 0
#endif

#ifndef MAX_LOG_LENGTH
    #define MAX_LOG_LENGTH 128
#endif

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
            log_to_file(debug_container->log_fp);
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

#if (LOG_LEVEL == 0)
    // Don't perform any logging at log level 0
    #define LOG(str, should_log, save) ((void) 0)
    #define LOG_FORMAT(format, data_type, data, should_log, save) ((void) 0)
    #define LOG_TO_FILE(should_log, save) ((void) 0)
#else
    #define LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_TO_FILE(should_log, save) log_to_file((should_log), (save))
#endif

#if DEBUG
    #define DEBUG_LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
#else
    #define DEBUG_LOG(str, should_log, save) ((void) 0)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) ((void) 0)
#endif

#endif