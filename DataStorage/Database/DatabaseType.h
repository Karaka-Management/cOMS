/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATABASE_TYPE_H
#define DATASTORAGE_DATABASE_TYPE_H

#include <string.h>

namespace DataStorage
{
    namespace Database
    {
        typedef enum {
            MYSQL = 1,
            SQLITE = 2,
            PGSQL = 3,
            SQLSRV = 4,
            UNDEFINED = 5
        } DatabaseType;

        DatabaseType database_type_from_str(const char* type)
        {
            if (strcmp(type, "mysql") == 0) {
                return DatabaseType::MYSQL;
            } else if (strcmp(type, "sqlite") == 0) {
                return DatabaseType::SQLITE;
            } else if (strcmp(type, "pqsql") == 0) {
                return DatabaseType::PGSQL;
            } else if (strcmp(type, "mssql") == 0) {
                return DatabaseType::SQLSRV;
            }
        	
            return DatabaseType::UNDEFINED;
        }
    }
}

#endif