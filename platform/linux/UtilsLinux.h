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
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../../stdlib/Types.h"
#include "../../utils/Utils.h"
#include "../../utils/TestUtils.h"

inline uint64 last_modification(const char* filename)
{
    struct stat buffer;
    stat(filename, &buffer);

    return (uint64) buffer.st_mtime.tv_sec;
}

inline void
file_read(const char* filename, file_body* file)
{
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);

    file->size = ftell(fp);
    rewind(fp);

    fread(file->content, 1, file->size, fp);

    fclose(fp);
}

inline
void strncpy_s(char *dest, size_t destsz, const char *src, size_t count) {
    size_t i;

    for (i = 0; i < count && i < destsz - 1 && src[i] != '\0'; ++i) {
        dest[i] = src[i];
    }

    dest[i] = '\0';
}
#endif