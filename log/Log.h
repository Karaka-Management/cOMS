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

void log_to_file();
void log(const char* str, bool should_log, const char* file, const char* function, int32 line);
void log(const char* format, LogDataArray data, bool should_log, const char* file, const char* function, int32 line);
void log_increment(int32, int64);
void log_counter(int32, int64);

#define LOG(should_log, str) log((str), (should_log), __FILE__, __func__, __LINE__)
#define LOG_FORMAT(should_log, format, ...) log((format), LogDataArray{__VA_ARGS__}, (should_log), __FILE__, __func__, __LINE__)
#define LOG_TO_FILE() log_to_file()

#if LOG_LEVEL == 2
    #define LOG_LEVEL_1(format, ...) log((format), LogDataArray{__VA_ARGS__}, true, __FILE__, __func__, __LINE__)
    #define LOG_LEVEL_2(format, ...) log((format), LogDataArray{__VA_ARGS__}, true, __FILE__, __func__, __LINE__)
#elif LOG_LEVEL == 1
    #define LOG_LEVEL_1(format, ...) log((format), LogDataArray{__VA_ARGS__}, true, __FILE__, __func__, __LINE__)
    #define LOG_LEVEL_2(format, ...) ((void) 0)
#elif LOG_LEVEL == 0
    #define LOG_LEVEL_1(format, ...) ((void) 0)
    #define LOG_LEVEL_2(format, ...) ((void) 0)
#endif

#if (!DEBUG && !INTERNAL) || RELEASE
    #define LOG_INCREMENT(a) ((void) 0)
    #define LOG_INCREMENT_BY(a, b) ((void) 0)
    #define LOG_COUNTER(a, b) ((void) 0)
    #define RESET_COUNTER(a) ((void) 0)
#else
    #define LOG_INCREMENT(a) log_increment((a), 1)
    #define LOG_INCREMENT_BY(a, b) log_increment((a), (b))
    #define LOG_COUNTER(a, b) log_counter((a), (b))
    #define RESET_COUNTER(a) reset_counter((a))
#endif

#endif