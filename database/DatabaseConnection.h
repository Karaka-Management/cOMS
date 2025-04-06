/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_DATABASE_CONNECTION_H
#define COMS_DATABASE_CONNECTION_H

#include "../stdlib/Types.h"
#include "DatabaseType.h"

struct DatabaseConnection {
    void* con;

    DatabaseType type;
    uint16 port;
    char* host;
    char* name; // databse name
};

#endif