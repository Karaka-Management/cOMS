/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef DATASTORAGE_DATABASE_PGSQL_CONNECTION_H
#define DATASTORAGE_DATABASE_PGSQL_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>

#include <libpq-fe.h>

#include "ConnectionAbstract.h"
#include "DbConnectionConfig.h"
#include "../DatabaseType.h"
#include "../DatabaseStatus.h"

namespace DataStorage::Database
{
    struct PostgresqlConnection : ConnectionAbstract {
        PostgresqlConnection(DbConnectionConfig dbdata)
        {
            this->type   = DatabaseType::PGSQL;
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

            char port[12];
            sprintf(port, "%d", this->dbdata.port);

            this->con = PQsetdbLogin(
                this->dbdata.host,
                port,
                NULL,
                NULL,
                this->dbdata.database,
                this->dbdata.login,
                this->dbdata.password
            );

            ConnStatusType stat = PQstatus((PGconn *) this->con);

            if (stat != ConnStatusType::CONNECTION_OK) {
                this->status = DatabaseStatus::MISSING_DATABASE;

                PQfinish((PGconn *) this->con);
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
                PQfinish((PGconn *) this->con);
            }

            this->con    = NULL;
            this->status = DatabaseStatus::CLOSED;
        }

        QueryResult query_execute(char *stmt, char *paramValues = NULL)
        {
            QueryResult result;

            PGresult *res;
            if (paramValues != NULL) {
                res = PQexecParams((PGconn *) this->con, stmt, 1, NULL, (const char* const*) paramValues, NULL, NULL, 0);
            } else {
                res = PQexec((PGconn *) this->con, stmt);
            }

            if (PQresultStatus(res) != PGRES_TUPLES_OK) {
                PQclear(res);

                return result;
            }

            result.rows    = PQntuples(res);
            result.columns = PQnfields(res);

            result.results = (char **) malloc(result.rows * result.columns * sizeof(char*));

            char *temp = NULL;
            size_t valLen = 0;

            for (size_t i = 0; i < result.rows; ++i) {
                for (size_t j = 0; j < result.columns; ++j) {
                    temp = PQgetvalue(res, i, j);

                    if (temp == NULL) {
                        result.results[i * result.columns + j] = NULL;

                        continue;
                    }

                    valLen = strlen(temp);

                    result.results[i * result.columns + j] = (char *) malloc((valLen + 1) * sizeof(char));
                    memcpy(result.results[i * result.columns + j], temp, valLen);
                    result.results[i * result.columns + j][valLen] = '\0';
                }
            }

            PQclear(res);

            return result;
        }
    };
}

#endif