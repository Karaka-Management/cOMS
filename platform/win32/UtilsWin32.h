/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_WIN32_H
#define TOS_UTILS_WIN32_H

#include <windows.h>
#ifdef _MSC_VER
    #include  <io.h>
#endif

#include "../../stdlib/Types.h"
#include "../../utils/Utils.h"
#include "../../utils/TestUtils.h"

#define strtok_r strtok_s

inline uint64
file_size(const char* filename)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(fp, &size)) {
        CloseHandle(fp);
    }

    CloseHandle(fp);

    return size.QuadPart;
}

inline void
file_read(const char* filename, file_body* file)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        file->size = 0;
        return;
    }

    LARGE_INTEGER size;
    if (!GetFileSizeEx(fp, &size)) {
        CloseHandle(fp);
        file->content = NULL;

        return;
    }

    DWORD bytes;
    ASSERT_SIMPLE(size.QuadPart < MAX_INT32);
    if (!ReadFile(fp, file->content, (uint32) size.QuadPart, &bytes, NULL)) {
        CloseHandle(fp);
        file->content = NULL;

        return;
    }

    CloseHandle(fp);

    file->content[bytes] = '\0';
    file->size = size.QuadPart;
}

inline uint64
file_read_struct(const char* filename, void* file, uint32 size)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return 0;
    }

    LARGE_INTEGER fsize;
    if (!GetFileSizeEx(fp, &fsize)) {
        CloseHandle(fp);

        return 0;
    }

    DWORD read;
    ASSERT_SIMPLE(fsize.QuadPart > size);
    if (!ReadFile(fp, file, (uint32) size, &read, NULL)) {
        CloseHandle(fp);

        return 0;
    }

    CloseHandle(fp);

    return read;
}

inline bool
file_write(const char* filename, const file_body* file)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    DWORD length = (DWORD) file->size;
    ASSERT_SIMPLE(file->size < MAX_INT32);
    if (!WriteFile(fp, file->content, length, &written, NULL)) {
        CloseHandle(fp);
        return false;
    }

    CloseHandle(fp);

    return true;
}

inline bool
file_write_struct(const char* filename, const void* file, uint32 size)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        GENERIC_WRITE,
        0,
        NULL,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    ASSERT_SIMPLE(size < MAX_INT32);
    if (!WriteFile(fp, file, size, &written, NULL)) {
        CloseHandle(fp);
        return false;
    }

    CloseHandle(fp);

    return true;
}

inline void
file_copy(const char* src, const char* dst)
{
    CopyFileA((LPCSTR) src, (LPCSTR) dst, false);
}

inline
HANDLE get_append_handle(const char* filename)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        FILE_APPEND_DATA,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    return fp;
}

inline bool
file_append(const char* filename, const char* file)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        FILE_APPEND_DATA,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    DWORD length = (DWORD) strlen(file); // @question WHY is WriteFile not supporting larger data?
    ASSERT_SIMPLE(length < MAX_INT32);
    if (!WriteFile(fp, file, length, &written, NULL)) {
        CloseHandle(fp);
        return false;
    }

    CloseHandle(fp);
    return true;
}

inline bool
file_append(HANDLE fp, const char* file)
{
    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    DWORD length = (DWORD) strlen(file); // @question WHY is WriteFile not supporting larger data?
    ASSERT_SIMPLE(length < MAX_INT32);
    if (!WriteFile(fp, file, length, &written, NULL)) {
        CloseHandle(fp);
        return false;
    }

    CloseHandle(fp);
    return true;
}

inline bool
file_append(const char* filename, const file_body* file)
{
    HANDLE fp = CreateFileA((LPCSTR) filename,
        FILE_APPEND_DATA,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);

    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD bytes;
    DWORD length = (DWORD) file->size;
    ASSERT_SIMPLE(file->size < MAX_INT32);
    if (!WriteFile(fp, file->content, length, &bytes, NULL)) {
        CloseHandle(fp);
        return false;
    }

    CloseHandle(fp);
    return true;
}

inline uint64 last_modified(const char* filename)
{
    FILETIME modified = {};

    WIN32_FIND_DATA find_data;
    HANDLE fp = FindFirstFileA(filename, (LPWIN32_FIND_DATAA) &find_data);
    if(fp != INVALID_HANDLE_VALUE) {
        modified = find_data.ftLastWriteTime;
        FindClose(fp);
    }

    ULARGE_INTEGER ull;
    ull.LowPart = modified.dwLowDateTime;
    ull.HighPart = modified.dwHighDateTime;

    return ull.QuadPart;
}

inline void self_path(char* path)
{
    //HMODULE dll = GetModuleHandle(NULL);
    GetModuleFileNameA(NULL, (LPSTR) path, MAX_PATH);
}

void log_to_file(LogPool* logs, HANDLE fp)
{
    // we don't log an empty log pool
    if (logs->pos == 0) {
        return;
    }

    char *offset = logs->memory;
    for (uint32 i = 0; i < logs->pos * MAX_LOG_LENGTH + MAX_LOG_LENGTH; ++i) {
        if (*offset == '\0') {
            *offset = '\n';

            // @performance would it make sense to jump to the next log message
            //          we know that after \0 until the end of this log message everything is 0
        }

        ++offset;
    }

    logs->memory[logs->count * MAX_LOG_LENGTH - 1] = '\0';
    file_append(fp, logs->memory);

    // reset log position to start of memory pool
    logs->pos = 0;
}

// snprintf(logs->memory + logs->pos * MAX_LOG_LENGTH, MAX_LOG_LENGTH, "My text %s", str1);
// log(log, NULL);
void log(LogPool* logs, HANDLE fp = NULL)
{
    // Zero memory after \0 until end of THIS log message
    // Older log messages that are coming after are retained
    // Older log messages can come after this log message due to the ring memory
    char *offset = logs->memory + logs->pos * MAX_LOG_LENGTH;
    bool ended = false;
    for (uint32 i = 0; i < MAX_LOG_LENGTH; ++i) {
        if (ended) {
            *offset = 0;
            ++offset;

            continue;
        }

        if (*offset == '\0') {
            ended = true;
        }

        ++offset;
    }

    ++logs->pos;
    // write log pool to file
    if (logs->pos >= logs->count) {
        if (fp != NULL) {
            log_to_file(logs, fp);
        }

        // reset log position to start of memory pool
        logs->pos = 0;
    }
}

#if (LOG_LEVEL == 0)
    // Don't perform any logging at log level 0
    #define LOG(logs, fp)
    #define LOG_TO_FILE(logs, fp)
#else
    #define LOG(logs, fp) log(logs, fp);
    #define LOG_TO_FILE(logs, fp) log_to_file(logs, fp);
#endif

#endif