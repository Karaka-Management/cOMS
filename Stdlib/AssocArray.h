/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef STDLIB_ASSOC_ARRAY_H
#define STDLIB_ASSOC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

namespace Stdlib {
    class AssocArray {
        private:

        public:
            typedef struct {
                int size;
                void **keys;
                void **values;
            } assoc_arr;

            static
            assoc_arr *hash_new(int size)
            {
                assoc_arr *h = (assoc_arr *) calloc(1, sizeof(assoc_arr));
                h->keys   = (void **) calloc(size, sizeof(void *));
                h->values = (void **) calloc(size, sizeof(void *));
                h->size   = size;

                return h;
            }

            static
            int hash_index(assoc_arr *h, void *key)
            {
                int i = (int) key % h->size;
                while (h->keys[i] && h->keys[i] != key) {
                    i = (i + 1) % h->size;
                }

                return i;
            }

            static
            void hash_insert(assoc_arr *h, void *key, void *value)
            {
                int i        = hash_index(h, key);
                h->keys[i]   = key;
                h->values[i] = value;
            }

            static
            void *hash_lookup(assoc_arr *h, void *key)
            {
                int i = hash_index(h, key);

                return h->values[i];
            }
    };
}

#endif