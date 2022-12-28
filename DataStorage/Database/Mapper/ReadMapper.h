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

namespace DataStorage
{
    namespace Database
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

            ReadMapper *where(char *where, void *data)
            {
                return this;
            }

            void *execute()
            {
                return NULL;
            }
        };
    }
}

#endif
