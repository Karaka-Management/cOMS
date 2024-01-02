/**
 * Jingga
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATBASE_MAPPER_ABSTRACT_H
#define DATASTORAGE_DATBASE_MAPPER_ABSTRACT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "../Connection/ConnectionAbstract.h"
#include "DataMapperTypes.h"

namespace DataStorage::Database
{
    struct MapperAbstract {
        const DataStorage::Database::MapperData *mapper = NULL;

        const DataStorage::Database::ConnectionAbstract *db = NULL;

        DataStorage::Database::MapperType type = DataStorage::Database::MapperType::MAPPER_DEFAULT;

        int MEMBERS = 0;

        int COLUMN_COUNT = 0;

        DataStorage::Database::ModelStructure *MODEL_STRUCTURE = NULL;

        char *PRIMARYFIELD = NULL;

        bool AUTOINCREMENT = true;

        char *CREATE_AT = NULL;

        char *TABLE = NULL;

        char *PARENT = NULL;

        char *MODEL = NULL;

        DataStorage::Database::DataMapperColumn *COLUMNS = NULL;

        DataStorage::Database::TableRelation *HAS_MANY = NULL;

        DataStorage::Database::TableRelation *OWNS_ONE = NULL;

        DataStorage::Database::TableRelation *BELONGS_TO = NULL;

        void *address(uintptr_t objPos, char *name)
        {
            uintptr_t pos = objPos;
            for (int i = 0; i < this->MEMBERS; ++i) {
                if (strcmp(this->MODEL_STRUCTURE[i].name, name) == 0) {
                    return (void *) pos;
                }

                pos += this->MODEL_STRUCTURE[i].size;
            }

            return NULL;
        }

        DataMapperColumn *findByColumnName(char *column)
        {
            for (int i = 0; i < this->COLUMN_COUNT; ++i) {
                if (strcmp(this->COLUMNS[i].name, column) == 0) {
                    return &this->COLUMNS[i];
                }
            }

            return NULL;
        }

        DataMapperColumn *findByMemberName(char *member)
        {
            for (int i = 0; i < this->COLUMN_COUNT; ++i) {
                if (strcmp(this->COLUMNS[i].internal, member) == 0) {
                    return &this->COLUMNS[i];
                }
            }

            return NULL;
        }

        MapperAbstract *where(
            char *member,
            void *value,
            int valueType,
            const char *logic = "=",
            const char *connector = "AND"
        ) {
            return this;
        }

        virtual void *execute(void *options = NULL);
    };
}

#endif
