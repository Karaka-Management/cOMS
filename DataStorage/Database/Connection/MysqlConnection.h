/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATABASE_MYSQL_CONNECTION_H
#define DATASTORAGE_DATABASE_MYSQL_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>

#include <mysql/mysql.h>

#include "ConnectionAbstract.h"
#include "DbConnectionConfig.h"
#include "../DatabaseType.h"
#include "../DatabaseStatus.h"

namespace DataStorage {
    namespace Database {
        struct MysqlConnection : ConnectionAbstract {
            MysqlConnection(DbConnectionConfig dbdata)
            {
                this->type   = DatabaseType::MYSQL;
                this->dbdata = dbdata;
            }

            void connect(DbConnectionConfig *dbdata = NULL)
            {
                this->dbdata = dbdata == NULL ? this->dbdata : *dbdata;

                if (this->dbdata.db == NULL
                    || this->dbdata.host == NULL
                    || this->dbdata.port == 0
                    || this->dbdata.database == NULL
                    || this->dbdata.login == NULL
                    || this->dbdata.password == NULL
                ) {
                    this->status = DatabaseStatus::FAILURE;

                    if (this->dbdata.password != NULL) {
                        free(this->dbdata.password);
                        this->dbdata.password = NULL;
                    }
                }

                this->close();
                this->con = mysql_init(NULL);

                ::MYSQL *stat = mysql_real_connect(
                    (::MYSQL *) this->con,
                    this->dbdata.host,
                    this->dbdata.login,
                    this->dbdata.password,
                    this->dbdata.database,
                    this->dbdata.port,
                    NULL, 0
                );

                if (!stat) {
                    this->status = DatabaseStatus::MISSING_DATABASE;

                    mysql_close((::MYSQL *) this->con);
                    this->con = NULL;

                    if (this->dbdata.password != NULL) {
                        free(this->dbdata.password);
                        this->dbdata.password = NULL;
                    }
                }
            }

            void close()
            {
                if (this->con != NULL) {
                    mysql_close((::MYSQL *) this->con);
                }

                this->con    = NULL;
                this->status = DatabaseStatus::CLOSED;
            }
        };
    }
}

#endif