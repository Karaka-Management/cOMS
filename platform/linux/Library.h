/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_LIBRARY_H
#define TOS_PLATFORM_LINUX_LIBRARY_H

#include <stdio.h>
#include <dlfcn.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "../../stdlib/Types.h"
#include "../../utils/StringUtils.h"
#include "UtilsLinux.h"
#include "../Library.h"

// @todo Rename file to Library.cpp

inline
bool library_load(Library* lib)
{
    size_t path_length = strlen(lib->dir);

    char dst[PATH_MAX];
    str_concat(
        lib->dir, path_length,
        lib->dst, strlen(lib->dst),
        dst
    );

    #if DEBUG
        char src[PATH_MAX];
        size_t dst_len = strlen(dst);

        memcpy(src, dst, dst_len + 1);

        memcpy(dst + dst_len - (sizeof(".so") - 1), "_temp", sizeof("_temp") - 1);
        memcpy(dst + dst_len - (sizeof(".so") - 1) + (sizeof("_temp") - 1), ".so", sizeof(".so"));

        lib->last_load = file_last_modified(src);
        file_copy(src, dst);
    #endif

    // Unload any previous instance of the library if it’s already loaded
    if (lib->handle) {
        dlclose(lib->handle);
        lib->handle = NULL;
        usleep(100000);  // 100 ms
    }

    // @question we might want RTLD_NOW?
    lib->handle = dlopen(dst, RTLD_LAZY);
    if (!lib->handle) {
        lib->is_valid = false;
        return lib->is_valid;
    }

    lib->is_valid = true;
    for (int32 c = 0; c < lib->function_count; ++c) {
        void* function = dlsym(lib->handle, lib->function_names[c]);
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
    if (lib->handle) {
        dlclose(lib->handle);
        lib->handle = NULL;
    }

    for (int32 c = 0; c < lib->function_count; ++c) {
        lib->functions[c] = NULL;
    }
}

#endif
