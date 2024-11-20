/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LIBRARY_H
#define TOS_PLATFORM_LIBRARY_H

#include "../stdlib/Types.h"

#if _WIN32
    #include <windows.h>
#endif

struct Library {
    #if _WIN32
        HMODULE handle;
    #elif __linux__
        void* handle;
    #endif

    bool is_valid;

    char dir[MAX_PATH];
    char dst[64];

    #if DEBUG
        uint64 last_load;
    #endif

    int32 function_count;
    const char** function_names;
    void** functions;
};

#endif