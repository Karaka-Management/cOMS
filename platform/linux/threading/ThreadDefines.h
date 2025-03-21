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

typedef struct {
    int32 futex;
} coms_pthread_mutex_t;

typedef void coms_pthread_mutexattr_t;
typedef void coms_pthread_condattr_t;
typedef void coms_pthread_rwlockattr_t;

typedef struct {
    int32 futex;
} coms_pthread_cond_t;

typedef struct {
    int32 futex;
    bool exclusive;
} coms_pthread_rwlock_t;

typedef int coms_pthread_t;

#endif