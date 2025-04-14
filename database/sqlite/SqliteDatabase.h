/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_DATABASE_SQLITE_H
#define COMS_DATABASE_SQLITE_H

#include "../../stdlib/Types.h"

#if _WIN32
    #include "../../EngineDependencies/sqlite/src/sqlite3.h"
#else
    #include <sqlite3.h>
#endif

inline
int32 db_open_sqlite(DatabaseConnection* con)
{
    int32 rc;
    rc = sqlite3_open(con->host, &con->db_sqlite);

    if (rc) {
        return rc;
    }

    return 0;
}

inline
void db_close_sqlite(DatabaseConnection* con)
{
    sqlite3_close(con->db_sqlite);
}

#endif