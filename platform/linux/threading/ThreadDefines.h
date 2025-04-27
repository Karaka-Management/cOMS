/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_LINUX_THREADING_THREAD_DEFINES_H
#define COMS_PLATFORM_LINUX_THREADING_THREAD_DEFINES_H

// #include <pthread.h>
// #include <unistd.h>

#include "../../../stdlib/Types.h"

#define THREAD_RETURN int32
typedef THREAD_RETURN (*ThreadJobFunc)(void*);

struct mutex {
    alignas(4) atomic_32 int32 futex;
};

typedef void mutexattr_t;
typedef void coms_pthread_condattr_t;
typedef void coms_pthread_rwlockattr_t;

struct mutex_cond {
    alignas(4) atomic_32 int32 futex;
} ;

struct coms_pthread_rwlock_t {
    alignas(4) atomic_32 int32 futex;
    bool exclusive;
};

struct coms_pthread_t {
    int h;
    void* stack;
};

#endif