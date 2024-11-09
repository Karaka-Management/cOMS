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
void wchar_to_char(wchar_t* str)
{
    char *src = (char*) str;
    char *dest = (char *) src;

    while (*src != '\0' && src[1] != '\0') {
        if (*src != '\0') {
            *dest++ = (char) *src;
        }

        ++src;
    }

    *dest = '\0';
}

inline
void wchar_to_char(const char* str, char* __restrict dest)
{
    char *src = (char*) str;

    while (*src != '\0' && src[1] != '\0') {
        if (*src != '\0') {
            *dest++ = (char) *src;
        }

        ++src;
    }

    *dest = '\0';
}

inline constexpr
int32 str_to_int(const char *str)
{
    int32 result = 0;

    int32 sign = 1;
    if (*str++ == '-') {
        sign = -1;
    }

    while (*str >= '0' && *str <= '9') {
        result *= 10;
        result += (*str - '0');

        ++str;
    }

    return result * sign;
}

inline constexpr
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
        if (thousands != '\0'
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

inline
size_t str_count(const char* __restrict str, const char* __restrict substr)
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

inline int64
str_concat(
    const char* src1,
    const char* src2,
    char* dst
) {
    int64 len = strlen(src1);
    int64 len_total = len;

    memcpy(dst, src1, len);
    dst += len;

    len = strlen(src2);
    memcpy(dst, src2, len);
    dst += len;

    *dst = '\0';

    return len_total + len;
}

inline int64
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

    return src1_length + src2_length;
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

inline constexpr
char toupper_ascii(char c)
{
    return c - 32 * (c >= 'a' && c <= 'z');
}

inline
void toupper_ascii(char* str)
{
    while (*str != '\0') {
        *str -= 32 * (*str >= 'a' && *str <= 'z');
        ++str;
    }
}

inline constexpr
char tolower_ascii(char c)
{
    return c + 32 * (c >= 'A' && c <= 'Z');
}

inline
void tolower_ascii(char* str)
{
    while (*str != '\0') {
        *str += 32 * (*str >= 'A' && *str <= 'Z');
        ++str;
    }
}

inline constexpr
void create_const_name(const unsigned char* name, char* modified_name)
{
    size_t i = 0;
    while (*name != '\0') {
        modified_name[i] = *name == ' ' ? '_' : toupper_ascii(*name);
        ++name;
        ++i;
    }

    modified_name[i] = '\0';
}

inline
void create_const_name(unsigned char* name)
{
    while (*name != '\0') {
        *name = *name == ' ' ? '_' : toupper_ascii(*name);
    }

    *name = '\0';
}

inline constexpr
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

void print_bytes(const void* ptr, size_t size)
{
    const unsigned char* bytePtr = (const unsigned char *) ptr;

    size_t count = 0;

    for (size_t i = 0; i < size; i++) {
        ++count;
        if (count == 1) {
            printf("%03lld - %03lld: %02x ", i + 1, i + 8, bytePtr[i]);
        } else if (count < 8) {
            printf("%02x ", bytePtr[i]);
        } else {
            printf("%02x\n", bytePtr[i]);
            count = 0;
        }
    }
}

inline constexpr
int32 is_eol(const char* str)
{
    if (*str == '\n') {
        return 1;
    } else if (*str == '\r' && str[1] == '\n') {
        return 2;
    }

    return 0;
}

inline constexpr
bool is_whitespace(char str)
{
    return str == ' ' || str == '\t';
}

inline
int32 chars_to_eol(const char* str)
{
    int32 offset = 0;
    while (!is_eol(str) && *str++ != '\0')  {
        ++offset;
    }

    return offset;
}

inline
int32 strcpy_to_eol(const char* src, char* dst)
{
    int32 offset = 0;
    while (!is_eol(src) && *src != '\0')  {
        *dst++ = *src++;
        ++offset;
    }

    *dst = '\0';

    return offset;
}

#endif