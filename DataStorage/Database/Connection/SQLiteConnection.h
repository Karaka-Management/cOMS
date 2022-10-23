/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATABASE_SQLITE_CONNECTION_H
#define DATASTORAGE_DATABASE_SQLITE_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>

#include <sqlite3.h>

#include "ConnectionAbstract.h"
#include "DbConnectionConfig.h"
#include "../DatabaseType.h"
#include "../DatabaseStatus.h"

namespace DataStorage {
    namespace Database {
        struct SQLiteConnection : ConnectionAbstract {
            SQLiteConnection(DbConnectionConfig dbdata)
            {
                this->type   = DatabaseType::MYSQL;
                this->dbdata = dbdata;
            }

            void connect(DbConnectionConfig *dbdata = NULL)
            {
                this->dbdata = dbdata == NULL ? this->dbdata : *dbdata;

                if (this->dbdata.db == NULL
                    || this->dbdata.database == NULL
                ) {
                    this->status = DatabaseStatus::FAILURE;

                    if (this->dbdata.password != NULL) {
                        free((char *) this->dbdata.password);
                        this->dbdata.password = NULL;
                    }
                }

                this->close();

                int stat = sqlite3_open(this->dbdata.host, (sqlite3 **) &this->con);
                if (stat != SQLITE_OK) {
                    this->status = DatabaseStatus::MISSING_DATABASE;

                    sqlite3_close((sqlite3 *) this->con);
                    this->con = NULL;

                    if (this->dbdata.password != NULL) {
                        free((char *) this->dbdata.password);
                        this->dbdata.password = NULL;
                    }
                }
            }

            void close()
            {
                if (this->con != NULL) {
                    sqlite3_close((sqlite3 *) this->con);
                }

                this->con    = NULL;
                this->status = DatabaseStatus::CLOSED;
            }
        };
    }
}

#endif