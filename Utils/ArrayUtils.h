/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef UTILS_ARRAY_UTILS_H
#define UTILS_ARRAY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "StringUtils.h"

namespace Utils
{
    namespace ArrayUtils
    {
        inline
        char* get_arg(const char *id, char **argv, int length)
        {
            if (Utils::StringUtils::is_number(id)) {
                return argv[atoi(id)];
            }

            for (int i = 0; i < length - 1; ++i) {
                if (strcmp(id, argv[i]) == 0) {
                    return i + 1 >= length ? NULL : argv[i + 1];
                }
            }

            return NULL;
        }

        inline
        bool has_arg(const char *id, char **argv, int length)
        {
            for (int i = 0; i < length; ++i) {
                if (strcmp(id, argv[i]) == 0) {
                    return true;
                }
            }

            return false;
        }
    }
}

#endif