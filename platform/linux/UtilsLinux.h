/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_LINUX_H
#define TOS_UTILS_LINUX_H

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../../stdlib/Types.h"

int32 sprintf_s(char *buffer, size_t sizeOfBuffer, const char *format, ...) {
    int32 result;
    va_list args;

    if (buffer == NULL || format == NULL || sizeOfBuffer == 0) {
        return -1;
    }

    va_start(args, format);
    result = vsnprintf(buffer, sizeOfBuffer, format, args);
    va_end(args);

    if (result >= 0 && (size_t) result >= sizeOfBuffer) {
        buffer[sizeOfBuffer - 1] = '\0';
        return 80;
    }

    // Return the result
    return result;
}

#endif