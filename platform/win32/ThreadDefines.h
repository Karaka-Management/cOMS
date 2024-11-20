/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREAD_DEFINES_H
#define TOS_PLATFORM_WIN32_THREAD_DEFINES_H

#include <windows.h>

typedef DWORD (WINAPI *ThreadJobFunc)(void*);
typedef CRITICAL_SECTION pthread_mutex_t;
typedef void pthread_mutexattr_t;
typedef void pthread_condattr_t;
typedef void pthread_rwlockattr_t;
typedef HANDLE pthread_t;
typedef CONDITION_VARIABLE pthread_cond_t;

struct pthread_rwlock_t {
    SRWLOCK lock;
    bool exclusive;
};

#define THREAD_RETURN DWORD WINAPI

#endif