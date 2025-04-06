/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_LINUX_NETWORK_SERVER_H
#define COMS_PLATFORM_LINUX_NETWORK_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>

#include "../../../stdlib/Types.h"
#include "../../../network/SocketConnection.h"
#include "../../../utils/EndianUtils.h"

void socket_non_blocking(SocketConnection* con)
{
    int flags = fcntl(con->sd, F_GETFL, 0);
    fcntl(con->sd, F_SETFL, flags | O_NONBLOCK);
}

// WARNING: requires `sudo setcap cap_net_raw=eip /path/to/your_program`
bool socket_server_raw_create(SocketConnection* con) {
    con->sd = socket(AF_INET6, SOCK_RAW, 255);

    int32 flags;
    if ((flags = fcntl(con->sd, F_GETFL, 0)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    if (fcntl(con->sd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (sockaddr *) &con->addr, sizeof(con->addr)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    return true;
}

// WARNING: requires `sudo setcap cap_net_raw=eip /path/to/your_program`
bool socket_server_udp_raw_create(SocketConnection* con) {
    con->sd = socket(AF_INET6, SOCK_RAW, IPPROTO_UDP);

    int32 flags;
    if ((flags = fcntl(con->sd, F_GETFL, 0)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    if (fcntl(con->sd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (sockaddr *) &con->addr, sizeof(con->addr)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    return true;
}

bool socket_server_udp_create(SocketConnection* con) {
    con->sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

    int32 flags;
    if ((flags = fcntl(con->sd, F_GETFL, 0)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    if (fcntl(con->sd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (sockaddr *) &con->addr, sizeof(con->addr)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    return true;
}

bool socket_server_http_create(SocketConnection* con)
{
    con->sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (con->sd < 0) {
        con->sd = 0;
        return false;
    }

    int32 opt = 1;
    setsockopt(con->sd, SOL_SOCKET, SO_REUSEADDR, (const char*) &opt, sizeof(opt));

    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (struct sockaddr *) &con->addr, sizeof(con->addr)) < 0) {
        close(con->sd);

        return false;
    }

    if (listen(con->sd, 5) < 0) {
        close(con->sd);
        con->sd = 0;

        return false;
    }

    return true;
}

bool socket_server_websocket_create(SocketConnection* con) {
    con->sd = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);

    int32 flags;
    if ((flags = fcntl(con->sd, F_GETFL, 0)) < 0 ||
        fcntl(con->sd, F_SETFL, flags | O_NONBLOCK) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    int opt = 1;
    setsockopt(con->sd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (sockaddr*)&con->addr, sizeof(con->addr)) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    if (listen(con->sd, SOMAXCONN) < 0) {
        close(con->sd);
        con->sd = 0;
        return false;
    }

    return true;
}

#endif