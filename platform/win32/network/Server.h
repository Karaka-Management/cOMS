/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_WIN32_NETWORK_SERVER_H
#define COMS_PLATFORM_WIN32_NETWORK_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <winsock2.h>
#include <ws2tcpip.h>

#include "../../../network/SocketConnection.h"
#include "../../../utils/EndianUtils.h"

#pragma comment(lib, "Ws2_32.lib")

void socket_server_udp_create(SocketConnection* con) {
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return;
    }

    // Create socket
    con->sd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
    if (con->sd == INVALID_SOCKET) {
        WSACleanup();
        return;
    }

    // Set socket to non-blocking mode
    u_long mode = 1;
    if (ioctlsocket(con->sd, FIONBIO, &mode) != NO_ERROR) {
        closesocket(con->sd);
        WSACleanup();
        return;
    }

    // Bind socket
    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (struct sockaddr *) &con->addr, sizeof(con->addr)) == SOCKET_ERROR) {
        closesocket(con->sd);
        WSACleanup();
        return;
    }
}

bool socket_server_http_create(SocketConnection* con)
{
    WSADATA wsaData;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    // Create socket
    con->sd = socket(AF_INET6, SOCK_STREAM, 0);
    if (con->sd == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    // Bind socket
    memset(&con->addr, 0, sizeof(con->addr));
    con->addr.sin6_family = AF_INET6;
    con->addr.sin6_addr = in6addr_any;
    con->addr.sin6_port = htons(con->port);

    if (bind(con->sd, (struct sockaddr *) &con->addr, sizeof(con->addr)) == SOCKET_ERROR) {
        closesocket(con->sd);
        WSACleanup();
        return false;
    }

    // Listen for incoming connections
    if (listen(con->sd, 5) < 0) {
        closesocket(con->sd);
        WSACleanup();
        return false;
    }

    return true;
}

#endif