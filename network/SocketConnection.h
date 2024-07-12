/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_NETWORK_SOCKET_CONNECTION_H
#define TOS_NETWORK_SOCKET_CONNECTION_H

#if _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <netdb.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif

struct SocketConnection {
    SOCKET sd;
    sockaddr_in6 server_addr;
    socklen_t addr_len;
};

#endif