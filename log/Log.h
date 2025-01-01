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
#include "../stdlib/Types.h"
#include "Debug.h"

#ifndef MAX_LOG_LENGTH
    #define MAX_LOG_LENGTH 1024
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

void log_to_file();
void log(const char* str, bool should_log, bool save, const char* file, const char* function, int32 line);
void log(const char* format, LogDataType data_type, void* data, bool should_log, bool save, const char* file, const char* function, int32 line);
void log_increment(int32, int64);
void log_counter(int32, int64);

#define LOG_PERFORMANCE_START(time_start)       \
({                                              \
    time_start = __rdtsc();                     \
})

#define LOG_PERFORMANCE_END(time_start)         \
({                                              \
    printf("%ld\n", __rdtsc() - (time_start));  \
})

#if (!DEBUG && !INTERNAL)
    // Don't perform any logging at log level 0
    #define LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_TO_FILE() ((void) 0)
    #define LOG_INCREMENT(a) ((void) 0)
    #define LOG_INCREMENT_BY(a, b) ((void) 0)
    #define LOG_COUNTER(a, b) ((void) 0)
    #define RESET_COUNTER(a) ((void) 0)
#else
    #define LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
    #define LOG_TO_FILE() log_to_file()
    #define LOG_INCREMENT(a) log_increment((a), 1)
    #define LOG_INCREMENT_BY(a, b) log_increment((a), (b))
    #define LOG_COUNTER(a, b) log_counter((a), (b))
    #define RESET_COUNTER(a) reset_counter((a))
#endif

#if DEBUG
    #define DEBUG_LOG(str, should_log, save) log((str), (should_log), (save), __FILE__, __func__, __LINE__)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) log((format), (data_type), (data), (should_log), (save), __FILE__, __func__, __LINE__)
#else
    #define DEBUG_LOG(str, should_log, save) ((void) 0)
    #define DEBUG_LOG_FORMAT(format, data_type, data, should_log, save) ((void) 0)
#endif

#endif