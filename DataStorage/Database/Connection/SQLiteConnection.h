/**
 * Jingga
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
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

namespace DataStorage::Database
{
    struct SQLiteConnection : ConnectionAbstract {
        SQLiteConnection(DbConnectionConfig dbdata)
        {
            this->type   = DatabaseType::MYSQL;
            this->dbdata = dbdata;
        }

        void connect(DbConnectionConfig *dbdata = NULL)
        {
            this->dbdata = dbdata == NULL ? this->dbdata : *dbdata;

            if (this->dbdata.db == DatabaseType::UNDEFINED
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

        QueryResult query_execute(char *stmt, char *paramValues = NULL)
        {
            QueryResult result;

            sqlite3_stmt *res;
            int status = 0;

            if (paramValues != NULL) {
                status = sqlite3_prepare_v2((sqlite3 *) this->con, stmt, -1, &res, 0);

                // @todo sqlite3_bind_int(res, 1, 3);
            } else {
                status = sqlite3_prepare_v2((sqlite3 *) this->con, stmt, -1, &res, 0);
            }

            if (res == NULL) {
                return result;
            }

            status = sqlite3_step(res);

            result.rows    = 100;
            result.columns = sqlite3_column_count(res);

            if (result.rows == 0 || result.columns == 0) {
                return result;
            }

            result.results = (char **) malloc(result.rows * result.columns * sizeof(char*));

            char *temp = NULL;
            size_t valLen = 0;
            char ** tempStorage = NULL;

            size_t i = 0;
            while (status == SQLITE_ROW) {
                if (i == result.rows) {
                    tempStorage = (char **) malloc((result.rows + 100) * result.columns * sizeof(char*));
                    memcpy(tempStorage, result.results, result.rows * result.columns * sizeof(char*));

                    free(result.results);
                    result.results = tempStorage;

                    result.rows += 100;
                }

                for (int j = 0; j < result.columns; ++j) {
                    temp = (char *) sqlite3_column_text(res, j);

                    if (temp == NULL) {
                        result.results[i * result.columns + j] = NULL;

                        continue;
                    }

                    valLen = strlen(temp);

                    result.results[i * result.columns + j] = (char *) malloc((valLen + 1) * sizeof(char));
                    memcpy(result.results[i * result.columns + j], temp, valLen);
                    result.results[i * result.columns + j][valLen] = '\0';
                }

                status = sqlite3_step(res);

                ++i;
            }

            --i;
            if (i != result.rows) {
                tempStorage = (char **) malloc(i * result.columns * sizeof(char*));
                memcpy(tempStorage, result.results, i * result.columns * sizeof(char*));

                free(result.results);
                result.results = tempStorage;

                result.rows = i;
            }

            sqlite3_finalize(res);

            return result;
        }
    };
}

#endif