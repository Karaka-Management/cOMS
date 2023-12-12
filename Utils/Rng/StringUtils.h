/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_RNG_STRING_UTILS_H
#define UTILS_RNG_STRING_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

namespace Utils::Rng::StringUtils
{
    inline
    char* generate_string(
        size_t min = 10, size_t max = 10,
        char *charset = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ", int charsetLength = 62
    ) {
        srand(time(0));

        size_t length = (rand() % (max - min + 1)) + min;
        char *randomString = (char *) malloc(length + 1);

        for (size_t i = 0; i < length; ++i) {
            randomString[i] = charset[rand() % charsetLength];
        }

        randomString[length] = '\0';

        return randomString;
    }
}

#endif