/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_UTILS_H
#define TOS_PLATFORM_WIN32_UTILS_H

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <time.h>

#ifdef _MSC_VER
    #include  <io.h>
#endif

#include "../../stdlib/Types.h"
#include "../../utils/Utils.h"
#include "../../utils/TestUtils.h"
#include "../../memory/RingMemory.h"
#include "../../log/Debug.cpp"

#define strtok_r strtok_s

void usleep(uint64 microseconds)
{
    if ((microseconds % 1000) == 0) {
        Sleep((DWORD) (microseconds / 1000));
        return;
    }

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);

    LARGE_INTEGER start, end;
    QueryPerformanceCounter(&start);
    long long target = start.QuadPart + (microseconds * frequency.QuadPart) / 1000000;

    do {
        QueryPerformanceCounter(&end);
    } while (end.QuadPart < target);
}

inline
time_t system_time()
{
    SYSTEMTIME systemTime;
    FILETIME fileTime;
    ULARGE_INTEGER largeInt;

    GetLocalTime(&systemTime);
    SystemTimeToFileTime(&systemTime, &fileTime);

    // Convert FILETIME to a 64-bit integer
    largeInt.LowPart = fileTime.dwLowDateTime;
    largeInt.HighPart = fileTime.dwHighDateTime;

    return ((time_t) (largeInt.QuadPart / 10000000ULL)) - ((time_t) 11644473600ULL);
}

// doesn't return clock time, only to return time since program start
// -> can be used for profiling
inline
uint64 time_mu()
{
    LARGE_INTEGER counter;
    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000000) / debug_container->performance_count_frequency;
}

inline
time_t unix_epoch_s()
{
    FILETIME ft;
    GetSystemTimeAsFileTime(&ft);

    ULARGE_INTEGER li;
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;

    time_t seconds_since_epoch = (li.QuadPart - 116444736000000000ULL) / 10000000ULL;

    return seconds_since_epoch;
}

// @todo Consider to implement directly mapped files (CreateFileMapping) for certain files (e.g. map data or texture data, ...)

inline
void relative_to_absolute(const char* rel, char* path)
{
    char self_path[MAX_PATH];
    int32 self_path_length = GetModuleFileNameA(NULL, self_path, MAX_PATH);
    if (self_path_length == 0) {
        return;
    }

    const char* temp = rel;
    if (temp[0] == '.' && temp[1] == '/') {
        temp += 2;
    }

    char* last = self_path + self_path_length;
    while (*last != '\\' && self_path_length > 0) {
        --last;
        --self_path_length;
    }

    ++self_path_length;

    memcpy(path, self_path, self_path_length);
    strcpy(path + self_path_length, temp);
}

// @todo Move file code to FileUtils.h
inline uint64
file_size(const char* path)
{
    // @performance Profile against fseek strategy
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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

inline
bool file_exists(const char* path)
{
    DWORD file_attr;

    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        file_attr = GetFileAttributesA(full_path);
    } else {
        file_attr = GetFileAttributesA(path);
    }

    return file_attr != INVALID_FILE_ATTRIBUTES;
}

inline void
file_read(const char* path, FileBody* file, RingMemory* ring = NULL)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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

    if (ring != NULL) {
        file->content = ring_get_memory(ring, size.QuadPart);
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
file_read_struct(const char* path, void* file, uint32 size)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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
file_write(const char* path, const FileBody* file)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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
file_write_struct(const char* path, const void* file, uint32 size)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
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
    if (*src == '.') {
        char src_full_path[MAX_PATH];
        relative_to_absolute(src, src_full_path);

        if (*dst == '.') {
            char dst_full_path[MAX_PATH];
            relative_to_absolute(dst, dst_full_path);

            CopyFileA((LPCSTR) src_full_path, (LPCSTR) dst_full_path, false);
        } else {
            CopyFileA((LPCSTR) src_full_path, (LPCSTR) dst, false);
        }
    } else if (*dst == '.') {
        char dst_full_path[MAX_PATH];
        relative_to_absolute(dst, dst_full_path);

        CopyFileA((LPCSTR) src, (LPCSTR) dst_full_path, false);
    } else {
        CopyFileA((LPCSTR) src, (LPCSTR) dst, false);
    }
}

inline
void close_handle(HANDLE fp)
{
    CloseHandle(fp);
}

inline
HANDLE get_append_handle(const char* path)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

    if (fp == INVALID_HANDLE_VALUE) {
        return NULL;
    }

    return fp;
}

bool file_append(const char* path, const char* file)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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
file_append(HANDLE fp, const char* file, size_t length)
{
    if (fp == INVALID_HANDLE_VALUE) {
        return false;
    }

    DWORD written;
    if (!WriteFile(fp, file, (uint32) length, &written, NULL)) {
        CloseHandle(fp);
        return false;
    }

    return true;
}

inline bool
file_append(const char* path, const FileBody* file)
{
    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = CreateFileA((LPCSTR) full_path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    } else {
        fp = CreateFileA((LPCSTR) path,
            FILE_APPEND_DATA,
            0,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL
        );
    }

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

inline
uint64 last_modified(const char* path)
{
    WIN32_FIND_DATA find_data;

    HANDLE fp;
    if (*path == '.') {
        char full_path[MAX_PATH];
        relative_to_absolute(path, full_path);

        fp = FindFirstFileA(full_path, (LPWIN32_FIND_DATAA) &find_data);
    } else {
        fp = FindFirstFileA(path, (LPWIN32_FIND_DATAA) &find_data);
    }

    FILETIME modified = {};
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
    GetModuleFileNameA(NULL, (LPSTR) path, MAX_PATH);
}

#endif