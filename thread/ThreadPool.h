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
    #include "../platform/win32/Thread.h"
#else
    #include "../platform/linux/Thread.h"
#endif

#include "../stdlib/Types.h"
#include "ThreadJob.h"

#if _WIN32

#elif __linux__
    #include "../platform/linux/Thread.h"
#endif

struct ThreadPool {
    ThreadJob *work_first;
    ThreadJob *work_last;

    pthread_mutex_t work_mutex;
    pthread_cond_t work_cond;
    pthread_cond_t working_cond;

    size_t working_cnt;
    size_t thread_cnt;

    int32 size;
    bool stop;
};

ThreadJob *thread_pool_work_poll(ThreadPool *pool)
{
    if (pool == NULL) {
        return NULL;
    }

    ThreadJob *work = pool->work_first;
    if (work == NULL) {
        return NULL;
    }

    if (work->next == NULL) {
        pool->work_first = NULL;
        pool->work_last  = NULL;
    } else {
        pool->work_first = work->next;
    }

    return work;
}

#ifdef _WIN32
static DWORD WINAPI thread_pool_worker(void* arg)
#else
static void* thread_pool_worker(void *arg)
#endif
{
    ThreadPool *pool = (ThreadPool *) arg;
    ThreadJob *work;

    while (true) {
        pthread_mutex_lock(&pool->work_mutex);

        while (pool->work_first == NULL && !pool->stop) {
            pthread_cond_wait(&pool->work_cond, &pool->work_mutex);
        }

        if (pool->stop) {
            break;
        }

        work = thread_pool_work_poll(pool);
        ++(pool->working_cnt);
        pthread_mutex_unlock(&pool->work_mutex);

        if (work != NULL) {
            work->func(work);
        }

        pthread_mutex_lock(&pool->work_mutex);
        --(pool->working_cnt);

        if (!pool->stop && pool->working_cnt == 0 && pool->work_first == NULL) {
            pthread_cond_signal(&pool->working_cond);
        }

        pthread_mutex_unlock(&pool->work_mutex);
    }

    --(pool->thread_cnt);
    pthread_cond_signal(&pool->working_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    return NULL;
}

ThreadPool *thread_pool_create(size_t num, ThreadPool* pool)
{
    pthread_t thread;
    size_t i;

    if (num == 0) {
        num = 2;
    }

    pool->thread_cnt = num;

    // @todo switch from pool mutex and pool cond to threadjob mutex/cond
    //      thread_pool_wait etc. should just itereate over all mutexes
    pthread_mutex_init(&pool->work_mutex, NULL);
    pthread_cond_init(&pool->work_cond, NULL);
    pthread_cond_init(&pool->working_cond, NULL);

    pool->work_first = NULL;
    pool->work_last  = NULL;

    for (i = 0; i < num; i++) {
        pthread_create(&thread, NULL, thread_pool_worker, pool);
        ++(pool->size);

        pthread_detach(thread);
    }

    return pool;
}

void thread_pool_wait(ThreadPool *pool)
{
    if (pool == NULL) {
        return;
    }

    pthread_mutex_lock(&pool->work_mutex);

    while (true) {
        if ((!pool->stop && pool->working_cnt != 0) || (pool->stop && pool->thread_cnt != 0)) {
            pthread_cond_wait(&pool->working_cond, &pool->work_mutex);
        } else {
            break;
        }
    }

    pthread_mutex_unlock(&pool->work_mutex);
}

void thread_pool_destroy(ThreadPool *pool)
{
    if (pool == NULL) {
        return;
    }

    pthread_mutex_lock(&pool->work_mutex);
    ThreadJob *work = pool->work_first;

    while (work != NULL) {
        work = work->next;
    }

    pool->stop = true;
    pthread_cond_broadcast(&pool->work_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    thread_pool_wait(pool);

    pthread_mutex_destroy(&pool->work_mutex);
    pthread_cond_destroy(&pool->work_cond);
    pthread_cond_destroy(&pool->working_cond);
}

ThreadJob* thread_pool_add_work(ThreadPool *pool, ThreadJob* job)
{
    if (pool == NULL) {
        return NULL;
    }

    if (job == NULL) {
        return NULL;
    }

    pthread_mutex_lock(&pool->work_mutex);
    if (pool->work_first == NULL) {
        pool->work_first = job;
        pool->work_last  = pool->work_first;
    } else {
        pool->work_last->next = job;
        pool->work_last       = job;
    }

    pthread_cond_broadcast(&pool->work_cond);
    pthread_mutex_unlock(&pool->work_mutex);

    return job;
}

#endif