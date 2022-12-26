#ifndef THREADS_JOB_H
#define THREADS_JOB_H

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <windows.h>
#endif

namespace Threads
{
    #ifdef _WIN32
        typedef DWORD WINAPI(*JobFunc)(void *);
    #else
        typedef void (*JobFunc)(void *);
    #endif

    struct job_t {
        JobFunc func;
        void *arg;
        job_t *next;
    };

    typedef job_t Job;
}

#endif