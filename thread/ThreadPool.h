/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_THREADS_THREAD_POOL_H
#define TOS_THREADS_THREAD_POOL_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

#include "../stdlib/Types.h"
#include "ThreadJob.h"
#include "ThreadOSWrapper.h"

typedef struct {
    ThreadJob *work_first;
    ThreadJob *work_last;

    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_cond_t working_cond;

    size_t working_cnt;
    size_t thread_cnt;

    int32 size;
    bool stop;
} ThreadPool;

#endif