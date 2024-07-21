/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_UTILS_STRING_UTILS_H
#define TOS_UTILS_STRING_UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../stdlib/Types.h"

inline size_t str_count(const char *str, const char *substr)
{
    size_t l1 = strlen(str);
    size_t l2 = strlen(substr);

    if (l2 == 0 || l1 < l2) {
        return 0;
    }

    size_t count = 0;
    for (str = strstr(str, substr); str; str = strstr(str + l2, substr)) {
        ++count;
    }

    return count;
}

inline char *strsep(const char **sp, const char *sep)
{
    char *p, *s;

    if (sp == NULL || *sp == NULL || **sp == '\0') {
        return (NULL);
    }

    s = (char *) *sp;
    p = s + strcspn(s, sep);

    if (*p != '\0') {
        *p++ = '\0';
    }

    *sp = p;

    return s;
}

inline void
str_concat(
    const char* src1, size_t src1_length,
    const char* src2, size_t src2_length,
    char* dst
) {
    memcpy(dst, src1, src1_length);
    dst += src1_length;

    memcpy(dst, src2, src2_length);
    dst += src2_length;

    *dst = '\0';
}

char *strtok(char *str, const char *delim, char **saveptr)
{
    if (str == NULL) {
        str = *saveptr;
    }

    if (str == NULL || *str == '\0') {
        return NULL;
    }

    char *token_start = str;
    char *token_end   = strpbrk(token_start, delim);

    if (token_end == NULL) {
        *saveptr = NULL;
    } else {
        *token_end = '\0';
        *saveptr   = token_end + 1;
    }

    return token_start;
}

inline
void format_number_render(int length, char* buffer, const char thousands = ',')
{
    int count = (int) (length / 3) - (length % 3 == 0 ? 1 : 0);

    int j = -1;
    for (int i = length; i > 0; --i) {
        ++j;

        if (j % 3 == 0 && j != 0) {
            buffer[i + count] = buffer[i];
            --count;
            buffer[i + count] = thousands;
        } else {
            buffer[i + count] = buffer[i];
        }
    }
}

char* format_number(size_t number, char* buffer, const char thousands = ',')
{
    int length = snprintf(buffer, 32, "%zu", number);
    format_number_render(length, buffer, thousands);

    return buffer;
}

char * format_number(int number, char* buffer, const char thousands = ',')
{
    int length = snprintf(buffer, 32, "%i", number);
    format_number_render(length, buffer, thousands);

    return buffer;
}

void create_const_name(const unsigned char *name, unsigned char* modified_name)
{
    // Print block
    if (name == NULL) {
        modified_name = NULL;
    } else {
        size_t i;
        const size_t length = strlen((const char *) name);
        for (i = 0; i < length; ++i) {
            modified_name[i] = name[i] == ' ' ? '_' : (unsigned char) toupper(name[i]);
        }

        modified_name[i] = '\0';
    }
}

/**
 * Custom implementation of strtok_r/strtok_s
 */
char* strtok_(char *str, const char *delim, char **key) {
    char *result;
    if (str == NULL) {
        str = *key;
    }

    str += strspn(str, delim);
    if (*str == '\0') {
        return NULL;
    }

    result = str;
    str += strcspn(str, delim);

    if (*str) {
        *str++ = '\0';
    }

    *key = str;

    return result;
}

#endif