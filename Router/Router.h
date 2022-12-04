/**
 * Karaka
 *
 * @package   Stdlib
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef ROUTER_ROUTER_H
#define ROUTER_ROUTER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <regex>

#include "../Stdlib/HashTable.h"
#include "../Hash/MeowHash.h"

namespace Router
{
    typedef void (*RouterFunc)(int, char **);

    typedef struct {
        Stdlib::HashTable::ht *routes;
    } Router;

    Router create_router(int size)
    {
        Router router;
        router.routes = Stdlib::HashTable::create_table(size, true);

        return router;
    }

    void set(Router *router, const char* route, void *endpoint)
    {
        Stdlib::HashTable::set_entry(router->routes, route, endpoint);
    }

    RouterFunc match_route(Router *router, const char *uri)
    {
        RouterFunc ptr = NULL;
        Stdlib::HashTable::it itr = Stdlib::HashTable::table_iterator(router->routes);

        std::regex regex;
        std::cmatch match;

        while (Stdlib::HashTable::next(&itr)) {
            regex = std::regex(itr.key);

            bool status = std::regex_search(uri, match, regex);
            if (status) {
                ptr = (RouterFunc) itr.value;
            }
        }

        return ptr;
    }

    void free_router(Router *router)
    {
        Stdlib::HashTable::free_table(router->routes);
        router->routes = NULL;
    }
}

#endif