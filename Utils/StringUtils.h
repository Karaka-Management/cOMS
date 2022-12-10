/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_STRING_UTILS_H
#define UTILS_STRING_UTILS_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "MathUtils.h"
#include "ArraySort.h"

namespace Utils
{
    namespace StringUtils
    {
        inline
        char *search_replace(const char *haystack, const char *needle, const char *replace)
        {
            const char *haystackT = haystack;

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
                    strcpy(&result[c], replace);

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

        inline
        bool is_number(const char *s)
        {
            while (*s != '\0') {
                if (!isdigit(*s)) {
                    return false;
                }

                ++s;
            }

            return true;
        }

        inline
        size_t str_count(char *str, char *substr)
        {
            int l1 = strlen(str);
            int l2 = strlen(substr);

            if (l2 == 0 || l1 < l2) {
                return 0;
            }

            int count = 0;
            for (str = strstr(str, substr); str; str = strstr(str + l2, substr)) {
                ++count;
            }

            return count;
        }

        char *strsep(char **sp, char *sep)
        {
            char *p, *s;

            if (sp == NULL || *sp == NULL || **sp == '\0') {
                return(NULL);
            }

            s = *sp;
            p = s + strcspn(s, sep);

            if (*p != '\0') {
                *p++ = '\0';
            }

            *sp = p;

            return(s);
        }

        int str_split(char **list, char *str, const char delim)
        {
            size_t splits = str_count(str, (char *) &delim) + 1;
            list          = (char **) malloc(splits * sizeof(char *));

            char *token;
            int i = 0;

            while ((token = strsep(&str, &delim)) != NULL) {
                list[i] = (char *) malloc(strlen(token + 1) * sizeof(char));
                memcpy(list[i], token, (strlen(token) + 1) * sizeof(char));

                ++i;
            }

            return i;
        }

        typedef struct {
            char **values;
            int *masks;
            int size;
        } text_diff;

        text_diff computeLCSDiff(char **from, int fromSize, char **to, int toSize)
        {
            char **diffValues = (char **) malloc(fromSize * toSize * sizeof(char *));
            int *diffMasks    = (int *) calloc(fromSize * toSize, sizeof(int));

            int *dm = (int *) calloc((fromSize + 1) * (toSize + 1), sizeof(int));

            if (!diffValues || !diffMasks || !dm) {
                fprintf(stderr, "CRITICAL: malloc failed");
            }

            int i = 0;
            int j = 0;

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

            int diffIndex = 0;

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

            char **diffValuesT = (char **) realloc(diffValues, diffIndex * sizeof(char *));
            if (!diffValuesT) {
                free(diffValues);
            }
            diffValues = diffValuesT;

            int *diffMasksT = (int *) realloc(diffMasks, diffIndex * sizeof(int));
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
    }
}

#endif