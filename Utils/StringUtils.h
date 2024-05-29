/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_STRING_UTILS_H
#define UTILS_STRING_UTILS_H

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "../Stdlib/Types.h"
#include "ArraySort.h"
#include "MathUtils.h"

namespace Utils::StringUtils
{
    char *search_replace(const char *haystack, const char *needle, const char *replace)
    {
        size_t i;
        size_t match     = 0;
        size_t oldLength = strlen(needle);
        size_t newLength = strlen(replace);

        for (i = 0; haystack[i] != '\0'; ++i) {
            if (strstr(&haystack[i], needle) == &haystack[i]) {
                ++match;

                i += oldLength - 1;
            }
        }

        char *result = (char *) malloc(i + match * (newLength - oldLength) + 1);
        if (!result) {
            return NULL;
        }

        size_t c = 0;
        while (*haystack && c < i + match * (newLength - oldLength)) {
            if (strstr(haystack, needle) == haystack) {
                strcpy(&result[c], (char *) replace);

                c        += newLength;
                haystack += oldLength;
            } else {
                result[c++] = *haystack++;
            }
        }

        if (c > i + match * (newLength - oldLength)) {
            c = i + match * (newLength - oldLength);
        }

        result[c] = '\0';

        return result;
    }

    inline bool is_number(const char *s)
    {
        while (*s != '\0') {
            if (!isdigit(*s)) {
                return false;
            }

            ++s;
        }

        return true;
    }

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

    // @todo Implement delim as const char* (also allow \0 length)
    inline int str_split(char **list, const char *str, const char delim)
    {
        size_t splits = str_count(str, (char *) &delim) + 1;
        list          = (char **) malloc(splits * sizeof(char *));

        char *token;
        int i = 0;

        while ((token = strsep(&str, (char *) &delim)) != NULL) {
            list[i] = (char *) malloc(strlen(token + 1) * sizeof(char));
            memcpy(list[i], token, (strlen(token) + 1) * sizeof(char));

            ++i;
        }

        return i;
    }

    // @todo Implement delim as const char* (also allow \0 length)
    inline char *str_combine(const char **str, size_t size, const char delim)
    {
        if (size < 1) {
            return NULL;
        }

        size_t total_size = 0;
        for (size_t i = 0; i < size; ++i) {
            total_size += strlen(str[i]);
        }

        // If delim ever becomes a string replace * 1 with * strlen(delimiter)
        total_size += (size - 1) * 1 + 1;

        char *result = (char *) malloc(total_size * sizeof(char));
        if (!result) {
            return NULL;
        }

        strcpy(result, str[0]);

        for (size_t i = 0; i < size; ++i) {
            strcat(result, &delim);
            strcat(result, str[i]);
        }

        return result;
    }

    struct text_diff {
        char **values;
        int64_t *masks;
        size_t size;
    };

    text_diff computeLCSDiff(const char **from, size_t fromSize, const char **to, size_t toSize)
    {
        char **diffValues  = (char **) malloc(fromSize * toSize * sizeof(char *));
        int64_t *diffMasks = (int64_t *) calloc(fromSize * toSize, sizeof(int64_t));

        int *dm = (int *) calloc((fromSize + 1) * (toSize + 1), sizeof(int));

        if (!diffValues || !diffMasks || !dm) {
            fprintf(stderr, "CRITICAL: malloc failed");
        }

        size_t i = 0;
        size_t j = 0;

        /* Handled with calloc
        for (j = 0; j <= toSize; ++j) {
            dm[0 + j] = 0;
        }

        for (i = 1; i <= fromSize; ++i) {
            dm[i * fromSize + 0] = 0;
        }
        */

        for (i = 1; i <= fromSize; ++i) {
            for (j = 1; j <= toSize; ++j) {
                dm[i * fromSize + j] = strcmp(from[i - 1], to[j - 1]) == 0
                                           ? dm[(i - 1) * fromSize + (j - 1)] + 1
                                           : oms_max(dm[(i - 1) * fromSize + j], dm[i * fromSize + (j - 1)]);
            }
        }

        size_t diffIndex = 0;

        i = fromSize;
        j = toSize;

        while ((i > 0 || j > 0) && diffIndex < fromSize * toSize) {
            if (j > 0 && dm[i * fromSize + (j - 1)] == dm[i * fromSize + j]) {
                diffValues[diffIndex] = (char *) malloc((strlen(to[j - 1]) + 1) * sizeof(char));
                if (!diffValues[diffIndex]) {
                    fprintf(stderr, "CRITICAL: malloc failed");

                    continue;
                }

#ifdef _WIN32
                strcpy_s(diffValues[diffIndex], (strlen(to[j - 1]) + 1) * sizeof(char), to[j - 1]);
#else
                strcpy(diffValues[diffIndex], to[j - 1]);
#endif

                diffMasks[diffIndex] = 1;

                --j;
                ++diffIndex;

                continue;
            }

            if (i > 0 && dm[(i - 1) * fromSize + j] == dm[i * fromSize + j]) {
                diffValues[diffIndex] = (char *) malloc((strlen(from[i - 1]) + 1) * sizeof(char));
                if (!diffValues[diffIndex]) {
                    fprintf(stderr, "CRITICAL: malloc failed");

                    continue;
                }

#ifdef _WIN32
                strcpy_s(diffValues[diffIndex], (strlen(from[i - 1]) + 1) * sizeof(char), from[i - 1]);
#else
                strcpy(diffValues[diffIndex], from[i - 1]);
#endif

                diffMasks[diffIndex] = -1;

                --i;
                ++diffIndex;

                continue;
            }

            diffValues[diffIndex] = (char *) malloc((strlen(from[i - 1]) + 1) * sizeof(char));
            if (!diffValues[diffIndex]) {
                fprintf(stderr, "CRITICAL: malloc failed");

                continue;
            }

#ifdef _WIN32
            strcpy_s(diffValues[diffIndex], (strlen(from[i - 1]) + 1) * sizeof(char), from[i - 1]);
#else
            strcpy(diffValues[diffIndex], from[i - 1]);
#endif

            /* Handled with calloc
            diffMasks[diffIndex] = 0;
            */

            --i;
            --j;
            ++diffIndex;
        }

        free(dm);

        // @todo do we even need to realloc?
        char **diffValuesT = (char **) realloc(diffValues, diffIndex * sizeof(char *));
        if (!diffValuesT) {
            free(diffValues);
        }
        diffValues = diffValuesT;

        int64_t *diffMasksT = (int64_t *) realloc(diffMasks, diffIndex * sizeof(int64_t));
        if (!diffMasksT) {
            free(diffMasks);
        }
        diffMasks = diffMasksT;

        if (!diffValues || !diffMasks) {
            fprintf(stderr, "CRITICAL: malloc failed");

            return text_diff{};
        }

        ArraySort::reverse_char(diffValues, diffIndex);
        ArraySort::reverse_int(diffMasks, diffIndex);

        return text_diff{diffValues, diffMasks, diffIndex};
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
} // namespace Utils::StringUtils

#endif