/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_LINUX_THREADING_THREAD_H
#define TOS_PLATFORM_LINUX_THREADING_THREAD_H

#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <linux/futex.h>
#include <sys/syscall.h>

#include "../../../stdlib/Types.h"
#include "../Allocator.h"
#include "ThreadDefines.h"

int32 pthread_create(pthread_t* thread, void *(*start_routine)(void *), void* arg) {
    thread->stack = platform_alloc_aligned(1 * MEGABYTE, 64);
    if (!thread->stack) {
        return -1;
    }

    thread->stack = clone(
        (int32 (*)(void *)) start_routine,
        stack + 1 * MEGABYTE,
        CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | SIGCHLD,
        arg
    );

    if (thread->stack == -1) {
        platform_aligned_free(thread->stack);

        return -1;
    }

    return 0;
}

int32 pthread_join(pthread_t thread, void** retval) {
    int32 status;
    if (waitpid(thread->id, &status, 0) == -1) {
        platform_aligned_free(thread->stack);

        return -1;
    }

    if (retval) {
        *retval = (void *) status;
    }

    platform_aligned_free(thread->stack);

    return 0;
}

int32 pthread_mutex_init(pthread_mutex_t* mutex, pthread_mutexattr_t*) {
    atomic_set(mutex, 0);

    return 0;
}

int32 pthread_mutex_lock(pthread_mutex_t* mutex) {
    int32 expected = 0;
    while (!atomic_compare_exchange_weak(mutex, &expected, 1)) {
        syscall(SYS_futex, mutex, FUTEX_WAIT, 1, NULL, NULL, 0);
        expected = 0;
    }

    return 0;
}

int32 pthread_mutex_unlock(pthread_mutex_t* mutex) {
    atomic_set(mutex, 0);
    syscall(SYS_futex, mutex, FUTEX_WAKE, 1, NULL, NULL, 0);

    return 0;
}

int32 pthread_cond_init(pthread_cond_t* cond, pthread_condattr_t*) {
    atomic_set(cond, 0);

    return 0;
}

int32 pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
    pthread_mutex_unlock(mutex);
    syscall(SYS_futex, cond, FUTEX_WAIT, atomic_get(cond), NULL, NULL, 0);
    pthread_mutex_lock(mutex);

    return 0;
}

int32 pthread_cond_signal(pthread_cond_t* cond) {
    atomic_add_fetch(cond, 1);
    syscall(SYS_futex, cond, FUTEX_WAKE, 1, NULL, NULL, 0);

    return 0;
}

int32 pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t*) {
    atomic_set((int64 *) &rwlock->readers, 0);
    //atomic_set(&rwlock->writer, 0);

    return 0;
}

int32 pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
    while (atomic_get(&rwlock->writer)) {}

    atomic_add_fetch(&rwlock->readers, 1);

    return 0;
}

int32 pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
    while (!atomic_compare_exchange_weak(&rwlock->writer, 0, 1)) {}

    return 0;
}

int32 pthread_rwlock_unlock(pthread_rwlock_t* rwlock) {
    if (atomic_get(&rwlock->writer)) {
        atomic_set(&rwlock->writer, 0);
    } else {
        atomic_sub_fetch(&rwlock->readers, 1);
    }

    return 0;
}

int32 pthread_detach(pthread_t) {
    // For detached threads, the OS will clean up automatically. We do nothing here.
    // Optionally, mark this thread as detached in your data structure if tracking threads.
    return 0;
}

int32 pthread_rwlock_destroy(pthread_rwlock_t*)
{
    return 0;
}

uint32 pthread_get_num_procs()
{
    return (uint32) sysconf(_SC_NPROCESSORS_ONLN);
}

#endif