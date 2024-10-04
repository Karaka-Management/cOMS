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

#ifdef _MSC_VER
    #include  <io.h>
#endif

#include "../../stdlib/Types.h"
#include "../../utils/Utils.h"
#include "../../utils/TestUtils.h"
#include "../../memory/RingMemory.h"

#define strtok_r strtok_s

inline void relative_to_absolute(const char* rel, char* path)
{
    char self_path[MAX_PATH];
    if (GetModuleFileNameA(NULL, self_path, MAX_PATH) == 0) {
        return;
    }

    const char* temp = rel;
    if (temp[0] == '.' && temp[1] == '/') {
        temp += 2;
    }

    char* last = strrchr(self_path, '\\');
    if (last != NULL) {
        *(last + 1) = '\0';
    }

    snprintf(path, MAX_PATH, "%s%s", self_path, temp);
}

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
uint64 time_ms()
{
    LARGE_INTEGER frequency;
    LARGE_INTEGER counter;

    if (!QueryPerformanceFrequency(&frequency)) {
        return 0;
    }

    QueryPerformanceCounter(&counter);

    return (counter.QuadPart * 1000000) / frequency.QuadPart;
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
    CopyFileA((LPCSTR) src, (LPCSTR) dst, false);
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