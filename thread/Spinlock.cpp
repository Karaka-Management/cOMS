/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_THREADS_SPINLOCK_C
#define TOS_THREADS_SPINLOCK_C

#if _WIN32
    #include "../platform/win32/threading/Spinlock.cpp"
#elif __linux__
    #include "../platform/linux/threading/Spinlock.cpp"
#endif

#endif