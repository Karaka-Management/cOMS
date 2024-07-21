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

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

#include "ThreadOSWrapper.h"
#include "ThreadJob.h"
#include "ThreadPool.h"

void thread_create(Worker* worker, ThreadJobFunc routine, void* arg)
{
    for (int i = 0; i < worker->mutex_size; ++i) {
        pthread_mutex_init(&worker->mutex[i], NULL);
    }

    pthread_cond_init(&worker->condition, NULL);
    pthread_create(&worker->thread, NULL, routine, arg);
}

void thread_stop(Worker* worker)
{
    pthread_join(worker->thread, NULL);
    pthread_cond_destroy(&worker->condition);

    for (int i = 0; i < worker->mutex_size; ++i) {
        pthread_mutex_destroy(&worker->mutex[i]);
    }
}

#endif