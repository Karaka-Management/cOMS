/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
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

namespace DataStorage::Database
{
    struct MysqlConnection : ConnectionAbstract {
        MysqlConnection(DbConnectionConfig dbdata)
        {
            this->type   = DatabaseType::MYSQL;
            this->dbdata = dbdata;
        }

        void connect(DbConnectionConfig *dbdata = NULL)
        {
            this->dbdata = dbdata == NULL ? this->dbdata : *dbdata;

            if (this->dbdata.db == DatabaseType::UNDEFINED
                || this->dbdata.host == NULL
                || this->dbdata.port == 0
                || this->dbdata.database == NULL
                || this->dbdata.login == NULL
                || this->dbdata.password == NULL
            ) {
                this->status = DatabaseStatus::FAILURE;

                if (this->dbdata.password != NULL) {
                    free((char *) this->dbdata.password);
                    this->dbdata.password = NULL;
                }
            }

            this->close();

            this->con = mysql_init(NULL);
            this->con = mysql_real_connect(
                (::MYSQL *) this->con,
                this->dbdata.host,
                this->dbdata.login,
                this->dbdata.password,
                this->dbdata.database,
                this->dbdata.port,
                NULL, 0
            );

            if (!this->con) {
                this->status = DatabaseStatus::MISSING_DATABASE;

                mysql_close((::MYSQL *) this->con);
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
                mysql_close((::MYSQL *) this->con);
            }

            this->con    = NULL;
            this->status = DatabaseStatus::CLOSED;
        }

        QueryResult query_execute(char *stmt, char *paramValues = NULL)
        {
            QueryResult result;

            MYSQL_RES *res;
            int status = 0;

            if (paramValues != NULL) {
                return result;
            } else {
                status = mysql_query((::MYSQL *) this->con, stmt);
                res = mysql_store_result((::MYSQL *) this->con);
            }

            if (res == NULL) {
                return result;
            }

            result.rows    = mysql_num_rows(res);
            result.columns = mysql_num_fields(res);

            if (result.rows == 0 || result.columns == 0) {
                return result;
            }

            result.results = (char **) malloc(result.rows * result.columns * sizeof(char*));

            char *temp = NULL;
            size_t valLen = 0;
            size_t *lengths;

            ::MYSQL_ROW row;
            size_t i = 0;

            while ((row = mysql_fetch_row(res)) != NULL) {
                lengths = mysql_fetch_lengths(res);
                for (int j = 0; j < result.columns; ++j) {
                    temp = row[j];

                    if (temp == NULL) {
                        result.results[i * result.columns + j] = NULL;

                        continue;
                    }

                    valLen = lengths[j];

                    result.results[i * result.columns + j] = (char *) malloc((valLen + 1) * sizeof(char));
                    memcpy(result.results[i * result.columns + j], temp, valLen);
                    result.results[i * result.columns + j][valLen] = '\0';
                }

                ++i;
            }

            return result;
        }
    };
}

#endif