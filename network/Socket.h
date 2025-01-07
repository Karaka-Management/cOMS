/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_NETWORK_SOCKET_H
#define TOS_NETWORK_SOCKET_H

#if _WIN32
    #include "../platform/win32/network/Socket.h"
#elif __linux__
    #include "../platform/linux/network/Socket.h"
#endif

#endif