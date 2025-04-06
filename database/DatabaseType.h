/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_DATABASE_TYPE_H
#define COMS_DATABASE_TYPE_H

enum DatabaseType : byte {
    DB_TYPE_SQLITE,
    DB_TYPE_MARIA,
    DB_TYPE_PSQL,
    DB_TYPE_MSSQL
};

#endif