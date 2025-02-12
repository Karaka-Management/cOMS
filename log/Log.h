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

#include "../stdlib/Types.h"
#include "../compiler/CompilerUtils.h"
#include "../utils/StringUtils.h"
#include "../platform/win32/TimeUtils.h"

#define LOG_DATA_ARRAY 5

#ifndef LOG_LEVEL
    #define LOG_LEVEL 0
#endif

#ifndef MAX_LOG_LENGTH
    #define MAX_LOG_LENGTH 256
#endif

#ifndef MAX_LOG_MESSAGES
    #define MAX_LOG_MESSAGES 256
#endif

#if _WIN32
    static HANDLE _log_fp;
#elif __linux__
    static int32 _log_fp;
#endif

struct LogMemory {
    byte* memory;

    uint64 size;
    uint64 pos;
};
static LogMemory* _log_memory = NULL;

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

// @bug This probably requires thread safety
byte* log_get_memory()
{
    if (_log_memory->pos + MAX_LOG_LENGTH > _log_memory->size) {
        _log_memory->pos = 0;
    }

    byte* offset = (byte *) (_log_memory->memory + _log_memory->pos);
    memset((void *) offset, 0, MAX_LOG_LENGTH);

    _log_memory->pos += MAX_LOG_LENGTH;

    return offset;
}

void log_to_file()
{
    // we don't log an empty log pool
    if (!_log_memory || _log_memory->pos == 0 || !_log_fp) {
        return;
    }

    #if _WIN32
        DWORD written;
        WriteFile(
            _log_fp,
            (char *) _log_memory->memory,
            (uint32) _log_memory->pos,
            &written,
            NULL
        );
    #else
        if (_log_fp < 0) {
            return;
        }

        write(
            _log_fp,
            (char *) _log_memory->memory,
            (uint32) _log_memory->pos
        );
    #endif
}

void log(const char* str, bool should_log, const char* file, const char* function, int32 line)
{
    if (!should_log || !_log_memory) {
        return;
    }

    int64 len = str_length(str);
    while (len > 0) {
        LogMessage* msg = (LogMessage *) log_get_memory();

        // Fill file
        msg->file = file;
        msg->function = function;
        msg->line = line;
        msg->message = (char *) (msg + 1);
        msg->time = system_time();

        int32 message_length = (int32) OMS_MIN(MAX_LOG_LENGTH - sizeof(LogMessage) - 1, len);

        memcpy(msg->message, str, message_length);
        msg->message[message_length] = '\0';
        str += message_length;
        len -= MAX_LOG_LENGTH - sizeof(LogMessage);

        if (_log_memory->size - _log_memory->pos < MAX_LOG_LENGTH) {
            log_to_file();
            _log_memory->pos = 0;
        }
    }
}

void log(const char* format, LogDataArray data, bool should_log, const char* file, const char* function, int32 line)
{
    ASSERT_SIMPLE(str_length(format) + str_length(file) + str_length(function) + 50 < MAX_LOG_LENGTH);

    if (!should_log || !_log_memory) {
        return;
    }

    if (data.data[0].type == LOG_DATA_VOID) {
        log(format, should_log, file, function, line);
        return;
    }

    LogMessage* msg = (LogMessage *) log_get_memory();
    msg->file = file;
    msg->function = function;
    msg->line = line;
    msg->message = (char *) (msg + 1);
    msg->time = system_time();

    char temp_format[MAX_LOG_LENGTH];
    str_copy_short(msg->message, format);

    for (int32 i = 0; i < LOG_DATA_ARRAY; ++i) {
        if (data.data[i].type == LOG_DATA_VOID) {
            break;
        }

        str_copy_short(temp_format, msg->message);

        switch (data.data[i].type) {
            case LOG_DATA_NONE: {
            }   break;
            case LOG_DATA_BYTE: {
                sprintf_fast_iter(msg->message, temp_format, (int32) *((byte *) data.data[i].value));
            } break;
            case LOG_DATA_INT32: {
                sprintf_fast_iter(msg->message, temp_format, *((int32 *) data.data[i].value));
            } break;
            case LOG_DATA_UINT32: {
                sprintf_fast_iter(msg->message, temp_format, *((uint32 *) data.data[i].value));
            } break;
            case LOG_DATA_INT64: {
                sprintf_fast_iter(msg->message, temp_format, *((int64 *) data.data[i].value));
            } break;
            case LOG_DATA_UINT64: {
                sprintf_fast_iter(msg->message, temp_format, *((uint64 *) data.data[i].value));
            } break;
            case LOG_DATA_CHAR: {
                sprintf_fast_iter(msg->message, temp_format, *((char *) data.data[i].value));
            } break;
            case LOG_DATA_CHAR_STR: {
                sprintf_fast_iter(msg->message, temp_format, (const char *) data.data[i].value);
            } break;
            case LOG_DATA_FLOAT32: {
                sprintf_fast_iter(msg->message, temp_format, *((f32 *) data.data[i].value));
            } break;
            case LOG_DATA_FLOAT64: {
                sprintf_fast_iter(msg->message, temp_format, *((f64 *) data.data[i].value));
            } break;
            default: {
                UNREACHABLE();
            }
        }
    }

    if (_log_memory->size - _log_memory->pos < MAX_LOG_LENGTH) {
        log_to_file();
        _log_memory->pos = 0;
    }
}

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

#endif