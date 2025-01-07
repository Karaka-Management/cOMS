/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_SYSTEM_INFO_C
#define TOS_SYSTEM_INFO_C

#if _WIN32
    #include "../platform/win32/SystemInfo.cpp"
#elif __linux__
    #include "../platform/linux/SystemInfo.cpp"
#endif

#endif