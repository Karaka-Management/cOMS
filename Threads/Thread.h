#ifndef THREADS_THREAD_H
#define THREADS_THREAD_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <pthread.h>
#endif

#include "OSWrapper.h"
#include "Job.h"
#include "ThreadPool.h"

namespace Threads {
    Job *pool_work_create(JobFunc func, void *arg)
    {
        Job *work;

        if (func == NULL) {
            return NULL;
        }

        work       = (Job *) malloc(sizeof(*work));
        work->func = func;
        work->arg  = arg;
        work->next = NULL;

        return work;
    }

    void pool_work_destroy(Job *work)
    {
        if (work == NULL) {
            return;
        }

        free(work);
    }

    Job *pool_work_get(Threads::ThreadPool *pool)
    {
        Job *work;

        if (pool == NULL) {
            return NULL;
        }

        work = pool->work_first;
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

    static void *pool_worker(void *arg)
    {
        Threads::ThreadPool *pool = (Threads::ThreadPool *) arg;
        Threads::Job *work;

        while (1) {
            pthread_mutex_lock(&(pool->work_mutex));

            while (pool->work_first == NULL && !pool->stop) {
                pthread_cond_wait(&(pool->work_cond), &(pool->work_mutex));
            }

            if (pool->stop) {
                break;
            }

            work = Threads::pool_work_get(pool);
            ++(pool->working_cnt);
            pthread_mutex_unlock(&(pool->work_mutex));

            if (work != NULL) {
                work->func(work->arg);
                pool_work_destroy(work);
            }

            pthread_mutex_lock(&(pool->work_mutex));
            --(pool->working_cnt);

            if (!pool->stop && pool->working_cnt == 0 && pool->work_first == NULL) {
                pthread_cond_signal(&(pool->working_cond));
            }

            pthread_mutex_unlock(&(pool->work_mutex));
        }

        --(pool->thread_cnt);
        pthread_cond_signal(&(pool->working_cond));
        pthread_mutex_unlock(&(pool->work_mutex));

        return NULL;
    }

    Threads::ThreadPool *pool_create(size_t num)
    {
        pthread_t thread;
        size_t i;

        if (num == 0) {
            num = 2;
        }

        Threads::ThreadPool *pool = (Threads::ThreadPool *) malloc(sizeof(Threads::ThreadPool));
        pool->thread_cnt          = num;

        pthread_mutex_init(&(pool->work_mutex), NULL);
        pthread_cond_init(&(pool->work_cond), NULL);
        pthread_cond_init(&(pool->working_cond), NULL);

        pool->work_first = NULL;
        pool->work_last  = NULL;

        for (i=0; i<num; i++) {
            pthread_create(&thread, NULL, pool_worker, pool);
            pthread_detach(thread);
        }

        return pool;
    }

    void pool_wait(Threads::ThreadPool *pool)
    {
        if (pool == NULL) {
            return;
        }

        pthread_mutex_lock(&(pool->work_mutex));

        while (true) {
            if ((!pool->stop && pool->working_cnt != 0) || (pool->stop && pool->thread_cnt != 0)) {
                pthread_cond_wait(&(pool->working_cond), &(pool->work_mutex));
            } else {
                break;
            }
        }

        pthread_mutex_unlock(&(pool->work_mutex));
    }

    void pool_destroy(Threads::ThreadPool *pool)
    {
        Threads::Job *work;
        Threads::Job *work2;

        if (pool == NULL) {
            return;
        }

        pthread_mutex_lock(&(pool->work_mutex));
        work = pool->work_first;

        while (work != NULL) {
            work2 = work->next;
            pool_work_destroy(work);
            work = work2;
        }

        pool->stop = true;
        pthread_cond_broadcast(&(pool->work_cond));
        pthread_mutex_unlock(&(pool->work_mutex));

        pool_wait(pool);

        pthread_mutex_destroy(&(pool->work_mutex));
        pthread_cond_destroy(&(pool->work_cond));
        pthread_cond_destroy(&(pool->working_cond));

        free(pool);
    }

    bool pool_add_work(Threads::ThreadPool *pool, JobFunc func, void *arg)
    {
        Threads::Job *work;

        if (pool == NULL) {
            return false;
        }

        work = Threads::pool_work_create(func, arg);
        if (work == NULL) {
            return false;
        }

        pthread_mutex_lock(&(pool->work_mutex));
        if (pool->work_first == NULL) {
            pool->work_first = work;
            pool->work_last  = pool->work_first;
        } else {
            pool->work_last->next = work;
            pool->work_last       = work;
        }

        pthread_cond_broadcast(&(pool->work_cond));
        pthread_mutex_unlock(&(pool->work_mutex));

        return true;
    }
}

#endif