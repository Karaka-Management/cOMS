/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATABASE_FIELD_H
#define DATASTORAGE_DATABASE_FIELD_H

#include <stdio.h>
#include <stdlib.h>

namespace DataStorage::Database
{
    typedef struct {
        char *name;
        char *type;
        void *def;
        bool is_nullable = true;
        bool is_primary = false;
        bool is_unique = false;
        bool autoincrement = false;
        char *foreignTable;
        char *foreignKey;
    } DbField;

    void free_DbField(DbField *field)
    {
        if (field->name != NULL) {
            free(field->name);
            field->name = NULL;
        }

        if (field->type != NULL) {
            free(field->type);
            field->type = NULL;
        }

        if (field->def != NULL) {
            free(field->def);
            field->def = NULL;
        }

        if (field->foreignTable != NULL) {
            free(field->foreignTable);
            field->foreignTable = NULL;
        }

        if (field->foreignKey != NULL) {
            free(field->foreignKey);
            field->foreignKey = NULL;
        }
    }
}

#endif