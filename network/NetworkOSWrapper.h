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
    #define close closesocket
    #define sleep Sleep
#endif

#endif