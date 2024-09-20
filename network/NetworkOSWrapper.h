/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_NETWORK_OS_WRAPPER_H
#define TOS_NETWORK_OS_WRAPPER_H

#if _WIN32
    #include <winsock2.h>
    #include <windows.h>

    inline
    int close(SOCKET sock) {
        return closesocket(sock);
    }

    inline
    void sleep(unsigned long time) {
        Sleep(time);
    }
#endif

#endif