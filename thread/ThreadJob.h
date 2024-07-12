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

#ifdef _WIN32
    #include <windows.h>

    typedef DWORD (WINAPI *ThreadJobFunc)(void*);
#else
    typedef void (*ThreadJobFunc)(void*);
#endif

#include "ThreadOSWrapper.h"

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