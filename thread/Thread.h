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
    #include "../platform/win32/threading/Thread.h"
    #include "../platform/win32/threading/Atomic.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
    #include "../platform/linux/threading/Atomic.h"
#endif

#include "ThreadJob.h"
#include "ThreadPool.h"

void thread_create(Worker* worker, ThreadJobFunc routine, void* arg)
{
    pthread_create(&worker->thread, NULL, routine, arg);
}

void thread_stop(Worker* worker)
{
    atomic_set(&worker->state, 0);
    pthread_join(worker->thread, NULL);
}

#endif