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

            char *database = NULL;

            char *host = NULL;

            int port = 0;

            char *login = NULL;

            char *password = NULL;
        } DbConnectionConfig;
    }
}

#endif