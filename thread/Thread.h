/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_THREADS_THREAD_H
#define COMS_THREADS_THREAD_H

#include <stdio.h>
#include "../stdlib/Types.h"
#include "../log/Log.h"
#include "Atomic.h"

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
#endif

#include "ThreadJob.h"
#include "ThreadPool.h"

void thread_create(Worker* worker, ThreadJobFunc routine, void* arg)
{
    LOG_1("Thread starting");
    coms_pthread_create(&worker->thread, NULL, routine, arg);
}

void thread_stop(Worker* worker)
{
    atomic_set_release(&worker->state, 0);
    coms_pthread_join(worker->thread, NULL);
    LOG_1("Thread ended");
}

#endif