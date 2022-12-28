/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATBASE_MAPPER_FACTORY_H
#define DATASTORAGE_DATBASE_MAPPER_FACTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../Connection/ConnectionAbstract.h"
#include "ReadMapper.h"
#include "MapperAbstract.h"

namespace DataStorage
{
    namespace Database
    {
        static DataStorage::Database::ConnectionAbstract *db;
        static char *dateTimeFormat = (char *) "Y-m-d H:i:s";

        struct DataMapperFactory {
            static ReadMapper *reader(const DataStorage::Database::MapperData *mapper, ConnectionAbstract *db = NULL)
            {
                ReadMapper *readMapper = (ReadMapper *) malloc(sizeof(ReadMapper));
                readMapper->mapper = mapper;
                readMapper->db = db == NULL ? DataStorage::Database::db : db;

                return readMapper;
            }

            static ReadMapper *get(const DataStorage::Database::MapperData *mapper, ConnectionAbstract *db = NULL)
            {
                ReadMapper *readMapper = (ReadMapper *) malloc(sizeof(ReadMapper));
                readMapper->mapper = mapper;
                readMapper->db = db == NULL ? DataStorage::Database::db : db;

                return readMapper->get();
            }

            static ReadMapper *getAll(const DataStorage::Database::MapperData *mapper, ConnectionAbstract *db = NULL)
            {
                ReadMapper *readMapper = (ReadMapper *) malloc(sizeof(ReadMapper));
                readMapper->mapper = mapper;
                readMapper->db = db == NULL ? DataStorage::Database::db : db;

                return readMapper->getAll();
            }
        };
    }
}

#endif
