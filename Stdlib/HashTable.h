/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef STDLIB_HASH_TABLE_H
#define STDLIB_HASH_TABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>

namespace Stdlib {
    namespace HashTable {
            typedef struct {
                const char *key;
                void *value;
            } entry;

            struct ht {
                bool is_fixed = false;
                entry *entries;
                size_t max;
                size_t size;
            };

            typedef struct {
                const char *key;
                void *value;

                ht *table;
                size_t index;
            } it;

            inline
            unsigned long long hash_key(const char *key)
            {
                unsigned long long hash = 14695981039346656037UL;
                for (const char *p = key; *p; ++p) {
                    hash ^= (unsigned long long)(unsigned char)(*p);
                    hash *= 1099511628211UL;
                }

                return hash;
            }

            ht *create_table(int max = 0, bool is_fixed = false)
            {
                ht *table = (ht *) malloc(sizeof(ht));
                if (table == NULL) {
                    return NULL;
                }

                table->size = 0;
                table->max  = max == 0 ? 16 : max;
                table->is_fixed = is_fixed;

                table->entries = (entry *) calloc(table->max, sizeof(entry));
                if (table->entries == NULL) {
                    free(table);
                    return NULL;
                }

                return table;
            }

            void *get_entry(ht *table, const char *key)
            {
                unsigned long long hash = hash_key(key);
                size_t index = (size_t)(hash & (unsigned long long)(table->max - 1));

                while (table->entries[index].key != NULL) {
                    if (strcmp(key, table->entries[index].key) == 0) {
                        return table->entries[index].value;
                    }

                    ++index;
                    if (index >= table->max) {
                        index = 0;
                    }
                }

                return NULL;
            }

            const char *_set_entry(entry *entries, size_t max, const char *key, void *value, size_t *size)
            {
                unsigned long long hash = hash_key(key);
                size_t index            = (size_t)(hash & (unsigned long long)(max - 1));

                while (entries[index].key != NULL) {
                    if (strcmp(key, entries[index].key) == 0) {
                        entries[index].value = value;

                        return entries[index].key;
                    }

                    ++index;
                    if (index >= max) {
                        index = 0;
                    }
                }

                if (size != NULL) {
                    #ifdef _WIN32
                        key = _strdup(key);
                    #else
                        key = strdup(key);
                    #endif

                    if (key == NULL) {
                        return NULL;
                    }

                    ++(*size);
                }

                entries[index].key = (char *) key;
                entries[index].value = value;

                return key;
            }

            bool expand_table(ht *table)
            {
                size_t new_max = table->max * 2;
                if (new_max < table->max) {
                    return false;
                }

                entry *new_entries = (entry *) calloc(new_max, sizeof(entry));
                if (new_entries == NULL) {
                    return false;
                }

                for (size_t i = 0; i < table->max; ++i) {
                    entry tmp = table->entries[i];
                    if (tmp.key != NULL) {
                        _set_entry(new_entries, new_max, tmp.key, tmp.value, NULL);
                    }
                }

                free(table->entries);
                table->entries = new_entries;
                table->max     = new_max;

                return true;
            }

            const char *set_entry(ht *table, const char *key, void *value)
            {
                if (value == NULL) {
                    return NULL;
                }

                if (table->is_fixed && table->size == table->max) {
                    return NULL;
                }

                if (!table->is_fixed && table->size >= table->max / 2) {
                    if (!expand_table(table)) {
                        return NULL;
                    }
                }

                return _set_entry(table->entries, table->max, key, value, &table->size);
            }

            it table_iterator(ht *table)
            {
                it it;
                it.table = table;
                it.index = 0;

                return it;
            }

            bool next(it *it)
            {
                ht *table = it->table;
                while (it->index < table->max) {
                    size_t i = it->index;
                    ++(it->index);

                    if (table->entries[i].key != NULL) {
                        entry tmp = table->entries[i];
                        it->key   = tmp.key;
                        it->value = tmp.value;

                        return true;
                    }
                }

                return false;
            }

            void free_table(ht *table)
            {
                if (!table || !table->entries) {
                    return;
                }

                for (size_t i = 0; i < table->max; ++i) {
                    if (table->entries[i].key) {
                        free((void *) table->entries[i].key);
                    }
                }

                free(table->entries);
            }
    };
}

#endif