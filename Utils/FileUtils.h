/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_FILE_UTILS_H
#define UTILS_FILE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
    #include <windows.h>
    #define _chdir chdir
    #define _getcwd getcwd

    #ifdef _MSC_VER
        #include  <io.h>
    #endif
#else
    #include <sys/stat.h>
    #include <unistd.h>
#endif

#include "TestUtils.h"

namespace Utils::FileUtils
{
    inline bool file_exists(const char* filename)
    {
    #ifdef _WIN32
        #ifdef _MSC_VER
        return _access(filename, 0) == 0;
        #endif
    #else
        struct stat buffer;
        return stat(filename, &buffer) == 0;
    #endif
    }

    inline void self_path(char* path)
    {
        #ifdef _WIN32
        HMODULE dll = GetModuleHandle(NULL);
        GetModuleFileNameA(dll, (LPSTR) path, MAX_PATH);
        #endif
    }

    inline uint64 last_modification(const char* filename)
    {
        #ifdef _WIN32
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
        #else
            struct stat buffer;
            stat(filename, &buffer);

            return (uint64) buffer.st_mtim.tv_sec;
        #endif
    }

    inline const char* file_extension(const char* filename)
    {
        char* dot = strrchr((char* ) filename, '.');

        if (!dot || dot == filename) {
            return "";
        }

        return dot + 1;
    }

    struct file_body {
        char* content;
        uint64 size = 0; // doesn't include null termination (same as strlen)
    };

    inline uint64
    file_size(const char* filename)
    {
        #ifdef _WIN32
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
        #endif
    }

    inline void
    file_read(const char* filename, file_body* file)
    {
        #ifdef _WIN32
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

        #endif
    }

    inline bool
    file_write(const char* filename, const file_body* file)
    {
        #ifdef _WIN32
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

        DWORD bytes;
        DWORD length = (DWORD) file->size;
        ASSERT_SIMPLE(file->size < MAX_INT32);
        if (!WriteFile(fp, file->content, length, &bytes, NULL)) {
            CloseHandle(fp);
            return false;
        }

        CloseHandle(fp);
        #endif

        return true;
    }

    inline void
    file_copy(const char* src, const char* dst)
    {
        #ifdef _WIN32
        CopyFileA((LPCSTR) src, (LPCSTR) dst, false);
        #endif
    }

    inline bool
    file_append(const char* filename, const file_body* file)
    {
        #ifdef _WIN32
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
        #endif

        return true;
    }
} // namespace Utils::FileUtils

#endif
