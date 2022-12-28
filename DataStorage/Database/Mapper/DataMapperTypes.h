/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATBASE_MAPPER_TYPES_H
#define DATASTORAGE_DATBASE_MAPPER_TYPES_H

#include <stdio.h>
#include <stdlib.h>

namespace DataStorage
{
    namespace Database
    {
        typedef enum {
            FIELD_TYPE_INT = 1,
            FIELD_TYPE_FLOAT = 2,
            FIELD_TYPE_STRING = 3,
            FIELD_TYPE_BOOL = 4
        } FieldType;

        typedef struct {
            const char *name = NULL;
            size_t size = 0;
        } ModelStructure;

        void free_ModelStructure(ModelStructure *data)
        {
            if (data->name != NULL) {
                free((void *) data->name);
            }
        }

        typedef struct {
            const char *name = NULL;
            int type = FieldType::FIELD_TYPE_INT;
            const char *internal = NULL;
            bool readonly = false;
            bool autocomplete = false;
        } DataMapperColumn;

        void free_DataMapperColumn(DataMapperColumn *data)
        {
            if (data->name != NULL) {
                free((void *) data->name);
            }

            if (data->internal != NULL) {
                free((void *) data->internal);
            }
        }

        typedef struct {
            char *member = NULL;
            char *mapper = NULL;
            char *external = NULL;
            char *table = NULL;
            char *self = NULL;
            char *column = NULL;
            bool conditional = false;
            char *by = NULL;
        } TableRelation;

        void free_TableRelation(TableRelation *data)
        {
            if (data->member != NULL) {
                free(data->member);
            }

            if (data->mapper != NULL) {
                free(data->mapper);
            }

            if (data->external != NULL) {
                free(data->external);
            }

            if (data->table != NULL) {
                free(data->table);
            }

            if (data->self != NULL) {
                free(data->self);
            }

            if (data->column != NULL) {
                free(data->column);
            }

            if (data->self != NULL) {
                free(data->self);
            }

            if (data->by != NULL) {
                free(data->by);
            }
        }

        typedef struct {
            const char *TABLE = NULL;
            const char *PRIMARYFIELD = NULL;
            const char *CREATED_AT = NULL;
            const char *PARENT = NULL;
            void *MODEL = NULL;

            int MEMBER_COUNT = 0;
            DataStorage::Database::ModelStructure *MODEL_STRUCTURE = NULL;

            int COLUMN_COUNT = 0;
            DataStorage::Database::DataMapperColumn *COLUMNS = NULL;

            int OWNS_ONE_COUNT = 0;
            DataStorage::Database::TableRelation *OWNS_ONE = NULL;

            int HAS_MANY_COUNT = 0;
            DataStorage::Database::TableRelation *HAS_MANY = NULL;

            int BELONGS_TO_COUNT = 0;
            DataStorage::Database::TableRelation *BELONGS_TO = NULL;
        } MapperData;

        void free_MapperData(DataStorage::Database::MapperData *data)
        {
            if (data->TABLE != NULL) {
                free((void *) data->TABLE);
            }

            if (data->PRIMARYFIELD != NULL) {
                free((void *) data->PRIMARYFIELD);
            }

            if (data->CREATED_AT != NULL) {
                free((void *) data->CREATED_AT);
            }

            if (data->PARENT != NULL) {
                free((void *) data->PARENT);
            }

            if (data->MODEL != NULL) {
                free(data->MODEL);
            }

            int i = 0;
            if (data->MODEL_STRUCTURE != NULL) {
                for (i = 0; i < data->MEMBER_COUNT; ++i) {
                    free_ModelStructure(&data->MODEL_STRUCTURE[i]);
                }

                free(data->MODEL_STRUCTURE);
            }

            if (data->COLUMNS != NULL) {
                for (i = 0; data->COLUMN_COUNT; ++i) {
                    free_DataMapperColumn(&data->COLUMNS[i]);
                }

                free(data->COLUMNS);
            }

            if (data->OWNS_ONE != NULL) {
                for (i = 0; data->OWNS_ONE_COUNT; ++i) {
                    free_TableRelation(&data->OWNS_ONE[i]);
                }

                free(data->OWNS_ONE);
            }

            if (data->HAS_MANY != NULL) {
                for (i = 0; data->HAS_MANY_COUNT; ++i) {
                    free_TableRelation(&data->HAS_MANY[i]);
                }

                free(data->HAS_MANY);
            }

            if (data->BELONGS_TO != NULL) {
                for (i = 0; data->BELONGS_TO_COUNT; ++i) {
                    free_TableRelation(&data->BELONGS_TO[i]);
                }

                free(data->BELONGS_TO);
            }
        }
    }
}

#endif
