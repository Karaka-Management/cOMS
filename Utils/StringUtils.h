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

namespace Utils {
    class StringUtils {
        private:

        public:
            static inline
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
    };
}

#endif