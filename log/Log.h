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
    int32 alignment;
    uint64 start;
    uint64 end;
};

void log_to_file();
void log(const char* str, bool should_log, bool save, const char* file, const char* function, int32 line);
void log(const char* format, LogDataType data_type, void* data, bool should_log, bool save, const char* file, const char* function, int32 line);

#if (LOG_LEVEL == 0)
    // Don't perform any logging at log level 0
    #define LOG(str, should_log, save) ((void) 0)
    #define LOG_FORMAT(format, data_type, data, should_log, save) ((void) 0)
    #define LOG_TO_FILE() ((void) 0)
#else
    #define LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_TO_FILE() log_to_file()
#endif

#if DEBUG
    #define DEBUG_LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
#else
    #define DEBUG_LOG(str, should_log, save) ((void) 0)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) ((void) 0)
#endif

#endif