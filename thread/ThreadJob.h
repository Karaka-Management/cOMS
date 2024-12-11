/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_THREADS_JOB_H
#define TOS_THREADS_JOB_H

#include <stdio.h>
#include <stdlib.h>

#include "../stdlib/Types.h"
#include "../memory/ThreadedRingMemory.h"

#if _WIN32
    #include "../platform/win32/threading/ThreadDefines.h"
#elif __linux__
    #include "../platform/linux/threading/ThreadDefines.h"
#endif

typedef void (*ThreadPoolJobFunc)(void*);

struct PoolWorker {
    int32 id;
    volatile int32 state;
    void* arg;
    void* result;
    RingMemory ring;
    ThreadPoolJobFunc func;
};

struct Worker {
    volatile int32 state;

    pthread_t thread;
    pthread_cond_t condition;
    int32 mutex_size;
    pthread_mutex_t* mutex;
};

#endif