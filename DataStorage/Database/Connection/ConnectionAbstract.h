/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATBASE_CONNECTION_ABSTRACT_H
#define DATASTORAGE_DATBASE_CONNECTION_ABSTRACT_H

#include <stdio.h>
#include <stdlib.h>

#include "DbConnectionConfig.h"
#include "../DatabaseType.h"
#include "../DatabaseStatus.h"

namespace DataStorage
{
    namespace Database
    {
        struct ConnectionAbstract {
            void *con = NULL;

            DbConnectionConfig dbdata;

            DatabaseType type = DatabaseType::UNDEFINED;

            DatabaseStatus status = DatabaseStatus::CLOSED;

            virtual void connect(DbConnectionConfig *dbdata = NULL)
            {
                return;
            }

            virtual void close()
            {
                if (this->con != NULL) {
                    free(con);
                }

                this->con    = NULL;
                this->status = DatabaseStatus::CLOSED;
            }
        };
    }
}

#endif