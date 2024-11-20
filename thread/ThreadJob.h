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

#if _WIN32
    #include "../platform/win32/ThreadDefines.h"
#elif __linux__
    #include "../platform/linux/ThreadDefines.h"
#endif

struct job_t {
    ThreadJobFunc func;
    void *arg;
    int state;
    job_t *next;
};

typedef job_t ThreadJob;

struct Worker {
    int index; // @todo When are we using this?
    int state;

    pthread_t thread;
    pthread_cond_t condition;
    int mutex_size;
    pthread_mutex_t* mutex;
};

#endif