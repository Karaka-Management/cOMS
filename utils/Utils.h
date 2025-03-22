/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_UTILS_H
#define COMS_UTILS_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/StringUtils.h"
#include "../compiler/CompilerUtils.h"

#if ARM
    #if ARM_NEON
        #include "../architecture/arm/utils/neon/Utils.h"
    #else
        #include "../architecture/arm/utils/sve/Utils.h"
    #endif
#else
    #include "../architecture/x86/simd/utils/Utils.h"
#endif

struct FileBody {
    uint64 size; // doesn't include null termination (same as strlen)
    byte* content;
};

FORCE_INLINE
bool is_equal(const byte* __restrict region1, const byte* __restrict region2, uint64 size) noexcept
{
    return memcmp(region1, region2, size) == 0;
}

inline
void str_output(const char* __restrict str, ...) {
    if (str_find(str, '%')) {
        va_list args;
        char buffer[1024];
        sprintf_fast(buffer, 1024, str, args);

        str = buffer;
    }

    #if _WIN32
        HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
        WriteFile(hStdout, str, str_length(str), NULL, NULL);
    else
        write(STDOUT_FILENO, str, str_length(str));
    #endif
}

inline
void swap_memory(void* __restrict a, void* __restrict b, size_t size) noexcept {
    byte* p = (byte*) a;
    byte* q = (byte*) b;

    // Swap in chunks of size_t
    while (size >= sizeof(size_t)) {
        size_t tmp = *(size_t *) p;
        *(size_t *) p = *(size_t *) q;
        *(size_t *) q = tmp;

        p += sizeof(size_t);
        q += sizeof(size_t);
        size -= sizeof(size_t);
    }

    // Swap remaining bytes
    while (size > 0) {
        byte tmp = *p;
        *p = *q;
        *q = tmp;

        ++p;
        ++q;
        --size;
    }
}

#endif