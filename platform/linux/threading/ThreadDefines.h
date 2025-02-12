/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_THREAD_DEFINES_H
#define TOS_PLATFORM_LINUX_THREADING_THREAD_DEFINES_H

// #include <pthread.h>
// #include <unistd.h>

#include "../../../stdlib/Types.h"

typedef void* (*ThreadJobFunc)(void*);
#define THREAD_RETURN void*

typedef volatile int32 pthread_mutex_t;
typedef volatile int32 pthread_cond_t;

struct pthread_t {
    int32 id;
    void* stack;
};

/*
struct pthread_mutex_t {
    volatile int32 lock;
};

struct pthread_cond_t {
    volatile int32 futex;
};
*/

struct pthread_rwlock_t {
    atomic_32 int32 readers;
    atomic_32 int32 writer;
};

typedef void pthread_mutexattr_t;
typedef void pthread_condattr_t;
typedef void pthread_rwlockattr_t;

// Thread local variable Already exists in c++11
// #define thread_local __thread

#endif