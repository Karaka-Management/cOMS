/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef APPLICATION_ABSTRACT_H
#define APPLICATION_ABSTRACT_H

#include <stdio.h>
#include <stdlib.h>

#include "../DataStorage/Database/Connection/ConnectionAbstract.h"
#include "../Utils/Parser/Json.h"
#include "../Threads/Thread.h"

namespace Application
{
    typedef struct {
        DataStorage::Database::ConnectionAbstract *db;
        nlohmann::json config;
        Threads::ThreadPool *pool;
    } ApplicationAbstract;
}

#endif
