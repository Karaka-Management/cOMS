/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_LINUX_NETWORK_SOCKET_H
#define COMS_PLATFORM_LINUX_NETWORK_SOCKET_H

#include "../../../stdlib/Types.h"

#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>

void socket_close(int32 sd) {
    shutdown(sd, SHUT_RDWR); 
    close(sd);
} 

#define socket_prepare() ((void)0)
#define socket_cleanup() ((void)0)

#endif