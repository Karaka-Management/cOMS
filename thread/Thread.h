/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_THREADS_THREAD_H
#define TOS_THREADS_THREAD_H

#include <stdio.h>
#include <stdlib.h>

#include "../stdlib/Types.h"

#if _WIN32
    #include "../platform/win32/ThreadDefines.h"
    #include "../platform/win32/Thread.h"
#elif __linux__
    #include "../platform/linux/ThreadDefines.h"
    #include "../platform/linux/Thread.h"
#endif

#include "ThreadJob.h"
#include "ThreadPool.h"

void thread_create(Worker* worker, ThreadJobFunc routine, void* arg)
{
    for (int32 i = 0; i < worker->mutex_size; ++i) {
        pthread_mutex_init(&worker->mutex[i], NULL);
    }

    pthread_cond_init(&worker->condition, NULL);
    pthread_create(&worker->thread, NULL, routine, arg);
}

void thread_stop(Worker* worker)
{
    atomic_set(&worker->state, 0);
    pthread_join(worker->thread, NULL);
    pthread_cond_destroy(&worker->condition);

    for (int32 i = 0; i < worker->mutex_size; ++i) {
        pthread_mutex_destroy(&worker->mutex[i]);
    }
}

#endif