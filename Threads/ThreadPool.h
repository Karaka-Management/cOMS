#ifndef THREADS_THREAD_POOL_H
#define THREADS_THREAD_POOL_H

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

namespace Threads
{
    typedef struct {
        Job *work_first;
        Job *work_last;
        pthread_mutex_t work_mutex;
        pthread_cond_t work_cond;
        pthread_cond_t working_cond;
        size_t working_cnt;
        size_t thread_cnt;
        bool stop;
    } ThreadPool;
}

#endif