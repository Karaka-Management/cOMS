/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_LOG_DEBUG_H
#define TOS_LOG_DEBUG_H

#include "../stdlib/Types.h"
#include "DebugMemory.h"
#include "TimingStat.h"
#include "../thread/Spinlock.h"

#if _WIN32
    #include <windows.h>
#endif

#ifndef MAX_LOG_LENGTH
    #define MAX_LOG_LENGTH 256
#endif

#ifndef MAX_LOG_MESSAGES
    #define MAX_LOG_MESSAGES 256
#endif

#ifndef LOG_LEVEL
    #define LOG_LEVEL 0
#endif

#define LOG_DATA_ARRAY 5

#ifndef DEBUG_COUNTER
    #define DEBUG_COUNTER 1
    enum DebugCounter {
        DEBUG_COUNTER_MEM_ALLOC,

        DEBUG_COUNTER_DRIVE_READ,
        DEBUG_COUNTER_DRIVE_WRITE,

        DEBUG_COUNTER_SIZE
    };
#endif

enum LogDataType {
    LOG_DATA_NONE,
    LOG_DATA_VOID,
    LOG_DATA_BYTE,
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

    uint64 size;
    uint64 pos;
};

struct LogMessage {
    const char* file;
    const char* function;
    int32 line;
    uint64 time;
    char* message;
};

struct LogData {
    LogDataType type;
    void* value;
};

struct LogDataArray{
    LogData data[LOG_DATA_ARRAY];
};

struct DebugContainer {
    DebugMemoryContainer dmc;

    // Used for logging timings for different sections
    TimingStat* perf_stats;
    spinlock32 perf_stats_spinlock;

    // Required to calculate the "fps"
    uint64 performance_count_frequency;

    // Used to log memory access (read, write)
    LogMemory log_memory;

    // Used to log general int values (e.g. counter for draw calls etc.)
    int64* counter;

    // We are not using FileHandle here since that would require us to include more files
    // These files in return require Debug.h
    #if _WIN32
        HANDLE log_fp;
    #elif __linux__
        int32 log_fp;
    #endif
};

#endif