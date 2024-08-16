/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_NETWORK_CLIENT_H
#define TOS_NETWORK_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SocketConnection.h"
#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include <winsock2.h>
    #include <windows.h>
#else
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

#include "NetworkOSWrapper.h"

#ifndef MAX_STATIC_NETWORK_PACKET_SIZE
    #define MAX_STATIC_NETWORK_PACKET_SIZE 8192
#endif

void socket_client_udb_connect(const char *hostname, SocketConnection* con) {
    addrinfo hints, *res, *p;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET6;
    hints.ai_socktype = SOCK_DGRAM;

    char port_str[6];
    snprintf(port_str, sizeof(port_str), "%d", con->port);

    if (getaddrinfo(hostname, port_str, &hints, &res) != 0) {
        return;
    }

    for (p = res; p != NULL; p = p->ai_next) {
        if ((con->sd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }

        memcpy((void *) &con->server_addr, p->ai_addr, p->ai_addrlen);
        break;
    }

    freeaddrinfo(res);
}

int socket_client_send(SOCKET sd, char *data, size_t length, sockaddr_in6 *server_addr, socklen_t addr_len) {
    int sent_bytes = sendto(sd, data, (int) length, 0, (sockaddr *)server_addr, addr_len);
    if (sent_bytes == -1) {
        return -1;
    }

    return 0;
}

int socket_client_disconnect(SOCKET sd)
{
    close(sd);

    return 0;
}

#endif