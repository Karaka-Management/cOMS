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

constexpr
size_t strlen_const(const char* str) {
    size_t len = 0;
    while (str[len] != '\0') {
        ++len;
    }

    return len;
}

inline
int32 utf8_encode(uint32 codepoint, char* out)
{
    if (codepoint <= 0x7F) {
        // 1-byte sequence: 0xxxxxxx
        out[0] = (byte) codepoint;

        return 1;
    } else if (codepoint <= 0x7FF) {
        // 2-byte sequence: 110xxxxx 10xxxxxx
        out[0] = 0xC0 | ((codepoint >> 6) & 0x1F);
        out[1] = 0x80 | (codepoint & 0x3F);

        return 2;
    } else if (codepoint <= 0xFFFF) {
        // 3-byte sequence: 1110xxxx 10xxxxxx 10xxxxxx
        out[0] = 0xE0 | ((codepoint >> 12) & 0x0F);
        out[1] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[2] = 0x80 | (codepoint & 0x3F);

        return 3;
    } else if (codepoint <= 0x10FFFF) {
        // 4-byte sequence: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        out[0] = 0xF0 | ((codepoint >> 18) & 0x07);
        out[1] = 0x80 | ((codepoint >> 12) & 0x3F);
        out[2] = 0x80 | ((codepoint >> 6) & 0x3F);
        out[3] = 0x80 | (codepoint & 0x3F);

        return 4;
    }

    return -1;
}

inline
int32 utf8_decode(const char* __restrict in, uint32* __restrict codepoint) {
    unsigned char ch = (unsigned char) *in;

    if (ch <= 0x7F) {
        // 1-byte sequence (ASCII)
        *codepoint = ch;

        return 1;
    } else if ((ch & 0xE0) == 0xC0) {
        // 2-byte sequence
        *codepoint = ((ch & 0x1F) << 6) | (in[1] & 0x3F);

        return 2;
    } else if ((ch & 0xF0) == 0xE0) {
        // 3-byte sequence
        *codepoint = ((ch & 0x0F) << 12) | ((in[1] & 0x3F) << 6) | (in[2] & 0x3F);

        return 3;
    } else if ((ch & 0xF8) == 0xF0) {
        // 4-byte sequence
        *codepoint = ((ch & 0x07) << 18) | ((in[1] & 0x3F) << 12) | ((in[2] & 0x3F) << 6) | (in[3] & 0x3F);

        return 4;
    }

    return -1;
}

inline
int32 utf8_strlen(const char* in) {
    int32 length = 0;
    int32 bytes;
    uint32 codepoint;

    while (*in) {
        bytes = utf8_decode(in, &codepoint);
        if (bytes < 0) {
            return -1;
        }

        in += bytes;
        ++length;
    }

    return length;
}

inline
void string_to_utf8(const uint32* in, char* out) {
    char buffer[5] = {0};
    while (*in) {
        int32 len = utf8_encode(*in, buffer);
        if (len > 0) {
            strncat(out, buffer, len);
        }

        ++in;
    }
}

inline
int32 utf8_get_char_at(const char* in, int32 index) {
    int32 i = 0;
    int32 bytes_consumed;
    uint32 codepoint;

    while (*in) {
        bytes_consumed = utf8_decode(in, &codepoint);
        if (bytes_consumed < 0) {
            return -1;
        }

        if (i == index) {
            return codepoint;
        }

        ++i;
        in += bytes_consumed;
    }

    return -1;
}

inline
void wchar_to_char(wchar_t* str, int32 length = 0)
{
    char *dest = (char *) str;
    size_t len = wcslen(str) * sizeof(wchar_t);

    if (length > 0 && length < len) {
        len = length;
    }

    for (int32 i = 0; i < len; ++i) {
        if (*str != '\0') {
            *dest = (char) *str;
            ++dest;
        }

        ++str;
    }

    *dest = '\0';
}

inline
void wchar_to_char(const wchar_t* __restrict src, char* __restrict dest, int32 length = 0)
{
    size_t len = wcslen(src) * sizeof(wchar_t);

    if (length > 0 && length < len) {
        len = length;
    }

    for (int32 i = 0; i < len; ++i) {
        if (*src != '\0') {
            *dest = (char) *src;
            ++dest;
        }

        ++src;
    }

    *dest = '\0';
}

inline
int32 str_to_int(const char *str)
{
    int32 result = 0;

    int32 sign = 1;
    if (*str == '-') {
        sign = -1;
        ++str;
    }

    while (*str >= '0' && *str <= '9') {
        result *= 10;
        result += (*str - '0');

        ++str;
    }

    return result * sign;
}

inline size_t str_count(const char* __restrict str, const char* __restrict substr)
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

inline char* strsep(const char* *sp, const char* sep)
{
    char* p, *s;

    if (sp == NULL || *sp == NULL || **sp == '\0') {
        return (NULL);
    }

    s = (char* ) *sp;
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

inline
char* strtok(char* str, const char* __restrict delim, char* *key) {
    char* result;
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

inline
int32 int_to_str(int64 number, char *str, const char thousands = ',') {
    int32 i = 0;
    int64 sign = number;
    int32 digit_count = 0;

    if (number == 0) {
        str[i++] = '0';
    } else if (number < 0) {
        number = -number;
    }

    while (number > 0) {
        if (thousands
            && (digit_count == 3 || digit_count == 6 || digit_count == 9 || digit_count == 12 || digit_count == 15)
        ) {
            str[i++] = thousands;
        }

        str[i++] = number % 10 + '0';
        number /= 10;
        ++digit_count;
    }

    if (sign < 0) {
        str[i++] = '-';
    }

    str[i] = '\0';

    for (int32 j = 0, k = i - 1; j < k; ++j, --k) {
        char temp = str[j];
        str[j] = str[k];
        str[k] = temp;
    }

    return i - 1;
}

char toupper_ascii(char c)
{
    return c >= 'a' && c <= 'z'
        ? c & 0x5f
        : c;
}

char tolower_ascii(char c)
{
    return c >= 'A' && c <= 'Z'
        ? c | 0x20
        : c;
}

void create_const_name(const unsigned char* name, char* modified_name)
{
    // Print block
    if (name == NULL) {
        modified_name = NULL;
    } else {
        size_t i;
        const size_t length = strlen((const char* ) name);
        for (i = 0; i < length; ++i) {
            modified_name[i] = name[i] == ' ' ? '_' : toupper_ascii(name[i]);
        }

        modified_name[i] = '\0';
    }
}

bool str_ends_with(const char* str, const char* suffix) {
    if (!str || !suffix) {
        return false;
    }

    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    if (suffix_len > str_len) {
        return false;
    }

    return strncmp(str + str_len - suffix_len, suffix, suffix_len) == 0;
}

// WARNING: result needs to have the correct length
void str_replace(const char* str, const char* __restrict search, const char* __restrict replace, char* result) {
    if (str == NULL || search == NULL || replace == NULL || result == NULL) {
        return;
    }

    size_t search_len = strlen(search);
    size_t replace_len = strlen(replace);

    if (search_len == 0) {
        strcpy(result, str);
        return;
    }

    const char* current = str;
    char* result_ptr = result;

    while ((current = strstr(current, search)) != NULL) {
        size_t bytes_to_copy = current - str;
        memcpy(result_ptr, str, bytes_to_copy);
        result_ptr += bytes_to_copy;

        memcpy(result_ptr, replace, replace_len);
        result_ptr += replace_len;

        current += search_len;
        str = current;
    }

    strcpy(result_ptr, str);
}

#endif