/**
 * Jingga
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATABASE_SCHEMA_H
#define DATASTORAGE_DATABASE_SCHEMA_H

#include <stdio.h>
#include <stdlib.h>

#include "DbField.h"

namespace DataStorage::Database
{
    struct DbSchema {
        char *name;
        DbField *fields;
        size_t field_size = 0;
    };

    void free_DbSchema(DbSchema *schema)
    {
        if (schema->name != NULL) {
            free(schema->name);
            schema->name = NULL;
        }

        if (schema->fields != NULL) {
            for (int i = 0; i < schema->field_size; ++i) {
                free_DbField(&schema->fields[i]);
            }

            free(schema->fields);
            schema->fields = NULL;
        }
    }
}

#endif