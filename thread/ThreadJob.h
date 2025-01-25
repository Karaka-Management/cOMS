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
#include "../thread/ThreadDefines.h"

typedef void (*ThreadPoolJobFunc)(void*);

struct PoolWorker {
    int32 id;
    int32 state;
    void* arg;
    void* result;
    RingMemory ring;
    ThreadPoolJobFunc func;
    ThreadPoolJobFunc callback;
};

struct Worker {
    int32 state;
    pthread_t thread;
};

#endif