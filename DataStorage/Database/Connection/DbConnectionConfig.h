/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATABASE_CONNECTION_H
#define DATASTORAGE_DATABASE_CONNECTION_H

#include <stdio.h>
#include <stdlib.h>

#include "../DatabaseType.h"

namespace DataStorage {
    namespace Database {
        typedef struct {
            DatabaseType db = DatabaseType::UNDEFINED;

            const char *database = NULL;

            const char *host = NULL;

            int port = 0;

            const char *login = NULL;

            const char *password = NULL;
        } DbConnectionConfig;

        void free_DbConnectionConfig(DbConnectionConfig *dbdata)
        {
            if (dbdata->database != NULL) {
                free((char *) dbdata->database);
                dbdata->database = NULL;
            }

            if (dbdata->host != NULL) {
                free((char *) dbdata->host);
                dbdata->host = NULL;
            }

            if (dbdata->login != NULL) {
                free((char *) dbdata->login);
                dbdata->login = NULL;
            }

            if (dbdata->password != NULL) {
                free((char *) dbdata->password);
                dbdata->password = NULL;
            }
        }
    }
}

#endif