/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_DATABASE_H
#define COMS_DATABASE_H

#include "../stdlib/Types.h"
#include "../../EngineDependencies/sqlite/src/sqlite3.h"

#include "DatabaseType.h"
#include "DatabaseConnection.h"

inline
int db_open_sqlite(DatabaseConnection* con)
{
    int rc;
    rc = sqlite3_open(con->host, &con->db_sqlite);

    if (rc) {
        return rc;
    }

    return 0;
}

inline
int db_open(DatabaseConnection* con)
{
    switch (con->type) {
        case DB_TYPE_SQLITE: {
            return db_open_sqlite(con);
        }
        case DB_TYPE_MARIA: {
            return 0;
        }
        case DB_TYPE_PSQL: {
            return 0;
        }
        case DB_TYPE_MSSQL: {
            return 0;
        }
    }

    return 0;
}

inline
void db_close_sqlite(DatabaseConnection* con)
{
    sqlite3_close(con->db_sqlite);
}

inline
void db_close(DatabaseConnection* con)
{
    switch (con->type) {
        case DB_TYPE_SQLITE: {
            db_close_sqlite(con);
            return;
        }
        case DB_TYPE_MARIA: {
            return;
        }
        case DB_TYPE_PSQL: {
            return;
        }
        case DB_TYPE_MSSQL: {
            return;
        }
    }
}

#endif