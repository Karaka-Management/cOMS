/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_LIBRARY_H
#define TOS_PLATFORM_WIN32_LIBRARY_H

#include <stdio.h>
#include <windows.h>
#include <string.h>

#include "../../stdlib/Types.h"
#include "UtilsWin32.h"
#include "../../utils/StringUtils.h"
#include "../Library.h"

// @todo Rename file to Library.cpp

inline
bool library_load(Library* lib)
{
    size_t path_length = strlen(lib->dir);

    char dst[MAX_PATH];
    str_concat(
        lib->dir, path_length,
        lib->dst, strlen(lib->dst),
        dst
    );

    #if DEBUG
        char src[MAX_PATH];
        size_t dst_len = strlen(dst);

        memcpy(src, dst, dst_len + 1);

        memcpy(dst + dst_len - (sizeof(".dll") - 1), "_temp", sizeof(".temp") - 1);
        memcpy(dst + dst_len - (sizeof(".dll") - 1) + (sizeof(".temp") - 1), ".dll", sizeof(".dll"));

        lib->last_load = last_modified(src);
        file_copy(src, dst);
    #endif

    // Make sure the dll is actually unloaded (Windows caches this)
    if (GetModuleHandleA((LPCSTR) dst)) {
        while (GetModuleHandleA((LPCSTR) dst) && lib->handle) {
            FreeLibrary(lib->handle);
            Sleep(100);
        }

        int32 i = 0;
        while (GetModuleHandleA((LPCSTR) dst) && i < 10) {
            ++i;
            Sleep(100);
        }
    }

    lib->handle = LoadLibraryA((LPCSTR) dst);
    if (!lib->handle) {
        lib->is_valid= false;

        return lib->is_valid;
    }

    lib->is_valid = true;
    for (int32 c = 0; c < lib->function_count; ++c) {
        void* function = (void *) GetProcAddress(lib->handle, (LPCSTR) lib->function_names[c]);
        if (function) {
            lib->functions[c] = function;
        } else {
            lib->is_valid = false;
        }
    }

    return lib->is_valid;
}

inline
void library_unload(Library* lib)
{
    FreeLibrary(lib->handle);
    for (int32 c = 0; c < lib->function_count; ++c) {
        lib->functions[c] = NULL;
    }
}

#endif