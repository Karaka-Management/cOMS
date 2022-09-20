/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATABASE_TYPE_H
#define DATASTORAGE_DATABASE_TYPE_H

namespace DataStorage {
    namespace Database {
        typedef enum {
            MYSQL = 0,
            SQLITE = 1,
            PGSQL = 2,
            SQLSRV = 3,
            UNDEFINED = 4
        } DatabaseType;
    }
}

#endif