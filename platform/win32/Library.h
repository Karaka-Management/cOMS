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

struct Library {
    HMODULE handle;
    uint64 last_load;
    bool is_valid;

    char dir[MAX_PATH];
    char src[64];
    char dst[64];

    uint32 function_count;
    const char** function_names;
    void** functions;
};

inline
bool library_load(Library* lib)
{
    size_t path_length = strlen(lib->dir);

    char src[MAX_PATH];
    str_concat(
        lib->dir, path_length,
        lib->src, strlen(lib->src),
        src
    );

    char dst[MAX_PATH];
    str_concat(
        lib->dir, path_length,
        lib->dst, strlen(lib->dst),
        dst
    );

    lib->last_load = last_modified(src);
    file_copy(src, dst);

    // Make sure the dll is actually unloaded (Windows caches this)
    if (GetModuleHandleA((LPCSTR) dst)) {
        while (GetModuleHandleA((LPCSTR) dst) && lib->handle) {
            FreeLibrary(lib->handle);
            Sleep(100);
        }

        int i = 0;
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
    for (int c = 0; c < lib->function_count; ++c) {
        void* function = GetProcAddress(lib->handle, lib->function_names[c]);
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
    for (int c = 0; c < lib->function_count; ++c) {
        lib->functions[c] = NULL;
    }
}

#endif