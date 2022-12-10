/**
 * Karaka
 *
 * @package   Models
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef DATASTORAGE_DATABASE_STATUS_H
#define DATASTORAGE_DATABASE_STATUS_H

namespace DataStorage
{
    namespace Database
    {
        typedef enum {
            OK = 0,
            MISSING_DATABASE = 1,
            MISSING_TABLE = 2,
            FAILURE = 3,
            READONLY = 4,
            CLOSED = 5
        } DatabaseStatus;
    }
}

#endif