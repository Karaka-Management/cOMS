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

#include "../Stdlib/AssocArray.h"
#include "StringUtils.h"

namespace Utils {
    class ArrayUtils {
        private:

        public:
            static inline
            char* get_arg (char *id, Stdlib::AssocArray::assoc_arr *args)
            {
                if (Utils::StringUtils::is_number(id)) {
                    return (char *) args->values[atoi(id)];
                }

                return (char *) Stdlib::AssocArray::hash_lookup(args, id);
            }

            static inline
            bool has_arg (char *id, Stdlib::AssocArray::assoc_arr *args)
            {
                return Stdlib::AssocArray::hash_lookup(args, id) == NULL;
            }
    };
}

#endif