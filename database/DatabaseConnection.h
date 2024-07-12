/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_DATABASE_CONNECTION
#define TOS_DATABASE_CONNECTION

#include "../stdlib/Types.h"
#include "../lib/sqlite/src/sqlite3.h"

#include "DatabaseType.h"

struct DatabaseConnection {
    union {
        sqlite3* db_sqlite;
        sqlite3* db_pgsql;
    };

    DatabaseType type;
    uint16 port;
    char* host;
    char* name; // databse name
};

#endif