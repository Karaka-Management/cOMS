/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATBASE_MAPPER_READ_H
#define DATASTORAGE_DATBASE_MAPPER_READ_H

#include <stdio.h>
#include <stdlib.h>

#include "../Connection/ConnectionAbstract.h"
#include "DataMapperFactory.h"
#include "MapperAbstract.h"

namespace DataStorage::Database
{
    struct ReadMapper : MapperAbstract {
        ReadMapper *get()
        {
            return this;
        }

        ReadMapper *getAll()
        {
            return this;
        }

        void *execute(void *options = NULL)
        {
            switch(this->type) {
                case DataStorage::Database::MapperType::MAPPER_GET:
                    return this->executeGet(options);
                case DataStorage::Database::MapperType::MAPPER_GET_RAW:
                    return this->executeGetRaw(options);
                case DataStorage::Database::MapperType::MAPPER_GET_ALL:
                    return this->executeGetAll(options);
                default:
                    return NULL;
            };
        }

        void *executeGet(void *options)
        {
            int *primaryKeys = NULL;
            const char *memberOfPrimaryField = findByColumnName((char *) this->mapper->PRIMARYFIELD)->internal;

            return NULL;
        }

        void *executeGetRaw(void *options)
        {
            return NULL;
        }

        void *executeGetAll(void *options)
        {
            return NULL;
        }
    };
}

#endif
