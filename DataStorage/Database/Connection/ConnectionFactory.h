/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATBASE_CONNECTION_FACTORY_H
#define DATASTORAGE_DATBASE_CONNECTION_FACTORY_H

#include <stdio.h>
#include <stdlib.h>

#include "../DatabaseType.h"

#include "ConnectionAbstract.h"
#include "DbConnectionConfig.h"
#include "MysqlConnection.h"
#include "PostgresqlConnection.h"
#include "SQLiteConnection.h"

namespace DataStorage
{
    namespace Database
    {
        ConnectionAbstract *create_connection(DbConnectionConfig dbdata)
        {
            switch (dbdata.db) {
                case DatabaseType::MYSQL:
                    return new MysqlConnection(dbdata);
                case DatabaseType::PGSQL:
                    return new PostgresqlConnection(dbdata);
                case DatabaseType::SQLSRV:
                    return NULL;
                case DatabaseType::SQLITE:
                    return new SQLiteConnection(dbdata);
                default:
                    return NULL;
            }
        }

        void close(ConnectionAbstract *db, DbConnectionConfig dbdata)
        {
            switch (dbdata.db) {
                case DatabaseType::MYSQL:
                    return ((MysqlConnection *) db)->close();
                case DatabaseType::PGSQL:
                    return ((PostgresqlConnection *) db)->close();
                case DatabaseType::SQLSRV:
                    return;
                case DatabaseType::SQLITE:
                    return ((SQLiteConnection *) db)->close();
                default:
                    return;
            }
        }
    }
}

#endif