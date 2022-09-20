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
#include <ctype.h>

#include "MathUtils.h"
#include "ArraySort.h"

namespace Utils {
    namespace StringUtils {
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

        typedef struct {
            char **values;
            int *masks;
            int size;
        } text_diff;

        text_diff computeLCSDiff (char **from, int fromSize, char **to, int toSize)
        {
            char **diffValues = (char **) malloc(fromSize * toSize * sizeof(char *));
            int *diffMasks    = (int *) calloc(fromSize * toSize,  sizeof(int));

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

            while (i > 0 || j > 0) {
                if (j > 0 && dm[i * fromSize + (j - 1)] == dm[i * fromSize + j]) {
                    diffValues[diffIndex] = (char *) malloc((strlen(to[j - 1]) + 1) * sizeof(char));
                    if (!diffValues[diffIndex]) {
                        fprintf(stderr, "CRITICAL: malloc failed");
                    }

                    strcpy(diffValues[diffIndex], to[j - 1]);
                    diffMasks[diffIndex] = 1;

                    --j;
                    ++diffIndex;

                    continue;
                }

                if (i > 0 && dm[(i - 1) * fromSize + j] == dm[i * fromSize + j]) {
                    diffValues[diffIndex] = (char *) malloc((strlen(from[i - 1]) + 1) * sizeof(char));
                    if (!diffValues[diffIndex]) {
                        fprintf(stderr, "CRITICAL: malloc failed");
                    }

                    strcpy(diffValues[diffIndex], from[i - 1]);
                    diffMasks[diffIndex] = -1;

                    --i;
                    ++diffIndex;

                    continue
                }

                diffValues[diffIndex] = (char *) malloc((strlen(from[i - 1]) + 1) * sizeof(char));
                if (!diffValues[diffIndex]) {
                    fprintf(stderr, "CRITICAL: malloc failed");
                }

                strcpy(diffValues[diffIndex], from[i - 1]);

                /* Handled with calloc
                diffMasks[diffIndex] = 0;
                */

                --i;
                --j;
                ++diffIndex;
            }

            free(dm);

            diffValues = (char **) realloc(diffValues, diffIndex * sizeof(char *));
            diffMasks  = (int *) realloc(diffMasks, diffIndex * sizeof(int));

            if (!diffValues || !diffMasks) {
                fprintf(stderr, "CRITICAL: malloc failed");
            }

            ArraySort::reverse_char(diffValues, diffIndex);
            ArraySort::reverse_int(diffMasks, diffIndex);

            return text_diff { diffValues, diffMasks, diffIndex  };
        }
    }
}

#endif