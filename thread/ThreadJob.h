/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_THREADS_JOB_H
#define COMS_THREADS_JOB_H

#include <stdio.h>
#include <stdlib.h>

#include "../stdlib/Types.h"
#include "../memory/ThreadedRingMemory.h"
#include "../thread/ThreadDefines.h"

typedef void (*ThreadPoolJobFunc)(void*);

struct PoolWorker {
    alignas(4) atomic_32 int32 id;
    alignas(4) atomic_32 int32 state;
    void* arg;
    void* result;
    RingMemory ring;
    ThreadPoolJobFunc func;
    ThreadPoolJobFunc callback;
};

struct Worker {
    alignas(4) atomic_32 int32 state;
    coms_pthread_t thread;
    void* arg;
};

#endif