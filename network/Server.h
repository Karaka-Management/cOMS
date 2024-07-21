/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_NETWORK_SERVER_H
#define TOS_NETWORK_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <fcntl.h>
    #include <errno.h>
#endif

#include "NetworkOSWrapper.h"
#include "SocketConnection.h"

void socket_server_udb_create(const char *hostname, SocketConnection* con) {
    con->sd = socket(AF_INET6, SOCK_DGRAM, 0);

    int flags;
    if ((flags = fcntl(con->sd, F_GETFL, 0)) < 0) {
        close(con->sd);
        con->sd = 0;
    }

    if (fcntl(con->sd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(con->sd);
        con->sd = 0;
    }

    con->server_addr.sin6_family = AF_INET6;
    con->server_addr.sin6_addr = in6addr_any;
    con->server_addr.sin6_port = htons(con->port);

    if (bind(con->sd, (sockaddr *) &con->server_addr, sizeof(sockaddr_in6)) < 0) {
        close(con->sd);
        con->sd = 0;
    }
}

#endif