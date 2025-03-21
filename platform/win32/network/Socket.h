/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_WIN32_NETWORK_SOCKET_H
#define COMS_PLATFORM_WIN32_NETWORK_SOCKET_H

#define socket_close(sd) closesocket(sd); WSACleanup()

#endif