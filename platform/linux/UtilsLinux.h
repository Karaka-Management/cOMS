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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/limits.h>

#include "../../stdlib/Types.h"
#include "../../utils/Utils.h"
#include "../../utils/TestUtils.h"

inline
uint64 file_size(const char* filename) {
    struct stat st;
    if (stat(filename, &st) != 0) {
        return 0;
    }

    return st.st_size;
}

inline
uint64 last_modified(const char* filename)
{
    struct stat buffer;
    stat(filename, &buffer);

    return (uint64) buffer.st_mtime;
}

inline
void file_read(const char* filename, file_body* file)
{
    FILE *fp = fopen(filename, "rb");
    fseek(fp, 0, SEEK_END);

    file->size = ftell(fp);
    rewind(fp);

    fread(file->content, 1, file->size, fp);

    fclose(fp);
}

inline
uint64_t file_read_struct(const char* filename, void* file, uint32 size) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return 0;
    }

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    ASSERT_SIMPLE(fsize > size);
    size_t read_bytes = fread(file, 1, size, fp);
    fclose(fp);

    return read_bytes;
}

inline
bool file_write(const char* filename, const file_body* file) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return false;
    }

    size_t written = fwrite(file->content, 1, file->size, fp);
    fclose(fp);

    return written == file->size;
}

inline
bool file_write_struct(const char* filename, const void* file, uint32_t size) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        return false;
    }

    size_t written = fwrite(file, 1, size, fp);
    fclose(fp);

    return written == size;
}

inline
void file_copy(const char* src, const char* dst) {
    FILE *src_fp = fopen(src, "rb");
    FILE *dst_fp = fopen(dst, "wb");

    if (!src_fp || !dst_fp) {
        if (src_fp) fclose(src_fp);
        if (dst_fp) fclose(dst_fp);
        return;
    }

    char buffer[4096];
    size_t bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), src_fp)) > 0) {
        fwrite(buffer, 1, bytes, dst_fp);
    }

    fclose(src_fp);
    fclose(dst_fp);
}

inline
FILE* get_append_handle(const char* filename) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        return NULL;
    }
    return fp;
}

inline bool file_append(const char* filename, const char* file) {
    FILE *fp = get_append_handle(filename);
    if (!fp) {
        return false;
    }

    size_t length = strlen(file);
    ASSERT_SIMPLE(length < INT32_MAX);
    size_t written = fwrite(file, 1, length, fp);
    fclose(fp);

    return written == length;
}

inline bool file_append(FILE* fp, const char* file) {
    if (!fp) {
        return false;
    }

    size_t length = strlen(file);
    ASSERT_SIMPLE(length < INT32_MAX);
    size_t written = fwrite(file, 1, length, fp);
    fclose(fp);

    return written == length;
}

inline bool file_append(const char* filename, const file_body* file) {
    FILE *fp = get_append_handle(filename);
    if (!fp) {
        return false;
    }

    size_t length = file->size;
    ASSERT_SIMPLE(length < INT32_MAX);
    size_t written = fwrite(file->content, 1, length, fp);
    fclose(fp);

    return written == length;
}

inline
void self_path(char* path) {
    size_t len = readlink("/proc/self/exe", path, PATH_MAX);
    if (len > 0) {
        path[len] = '\0';
    } else {
        path[0] = '\0';
    }
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