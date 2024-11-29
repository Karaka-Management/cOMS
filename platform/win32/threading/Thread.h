/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_PLATFORM_WIN32_THREADING_THREAD_H
#define TOS_PLATFORM_WIN32_THREADING_THREAD_H

#include "../../../stdlib/Types.h"
#include "../TimeUtils.h"
#include "ThreadDefines.h"

#include <windows.h>

int32 pthread_create(pthread_t* thread, void*, ThreadJobFunc start_routine, void* arg)
{
    if (thread == NULL || start_routine == NULL) {
        return 1;
    }

    *thread = CreateThread(NULL, 0, start_routine, arg, 0, NULL);
    if (*thread == NULL) {
        return 1;
    }

    return 0;
}

int32 pthread_join(pthread_t thread, void**)
{
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);

    return 0;
}

int32 pthread_detach(pthread_t thread)
{
    CloseHandle(thread);

    return 0;
}

int32 pthread_mutex_init(pthread_mutex_t* mutex, pthread_mutexattr_t*)
{
    if (mutex == NULL) {
        return 1;
    }

    InitializeCriticalSection(mutex);

    return 0;
}

int32 pthread_mutex_destroy(pthread_mutex_t* mutex)
{
    if (mutex == NULL) {
        return 1;
    }

    DeleteCriticalSection(mutex);

    return 0;
}

int32 pthread_mutex_lock(pthread_mutex_t* mutex)
{
    if (mutex == NULL) {
        return 1;
    }

    EnterCriticalSection(mutex);

    return 0;
}

int32 pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    if (mutex == NULL) {
        return 1;
    }

    LeaveCriticalSection(mutex);

    return 0;
}

int32 pthread_cond_init(pthread_cond_t* cond, pthread_condattr_t*)
{
    if (cond == NULL) {
        return 1;
    }

    InitializeConditionVariable(cond);

    return 0;
}

int32 pthread_cond_destroy(pthread_cond_t*)
{
    /* Windows does not have a destroy for conditionals */
    return 0;
}

int32 pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abstime)
{
    if (cond == NULL || mutex == NULL) {
        return 1;
    }

    if (!SleepConditionVariableCS(cond, mutex, timespec_to_ms(abstime))) {
        return 1;
    }

    return 0;
}

int32 pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex)
{
    if (cond == NULL || mutex == NULL) {
        return 1;
    }

    return pthread_cond_timedwait(cond, mutex, NULL);
}

int32 pthread_cond_signal(pthread_cond_t* cond)
{
    if (cond == NULL) {
        return 1;
    }

    WakeConditionVariable(cond);

    return 0;
}

int32 pthread_cond_broadcast(pthread_cond_t* cond)
{
    if (cond == NULL) {
        return 1;
    }

    WakeAllConditionVariable(cond);

    return 0;
}

int32 pthread_rwlock_init(pthread_rwlock_t* rwlock, const pthread_rwlockattr_t*)
{
    if (rwlock == NULL) {
        return 1;
    }

    InitializeSRWLock(&rwlock->lock);
    rwlock->exclusive = false;

    return 0;
}

int32 pthread_rwlock_destroy(pthread_rwlock_t*)
{
    return 0;
}

int32 pthread_rwlock_rdlock(pthread_rwlock_t* rwlock)
{
    if (rwlock == NULL) {
        return 1;
    }

    AcquireSRWLockShared(&rwlock->lock);

    return 0;
}

int32 pthread_rwlock_tryrdlock(pthread_rwlock_t* rwlock)
{
    if (rwlock == NULL) {
        return 1;
    }

    return !TryAcquireSRWLockShared(&rwlock->lock);
}

int32 pthread_rwlock_wrlock(pthread_rwlock_t* rwlock)
{
    if (rwlock == NULL) {
        return 1;
    }

    AcquireSRWLockExclusive(&rwlock->lock);
    rwlock->exclusive = true;

    return 0;
}

int32 pthread_rwlock_trywrlock(pthread_rwlock_t  *rwlock)
{
    if (rwlock == NULL) {
        return 1;
    }

    BOOLEAN ret = TryAcquireSRWLockExclusive(&rwlock->lock);
    if (ret) {
        rwlock->exclusive = true;
    }

    return ret;
}

int32 pthread_rwlock_unlock(pthread_rwlock_t* rwlock)
{
    if (rwlock == NULL) {
        return 1;
    }

    if (rwlock->exclusive) {
        rwlock->exclusive = false;
        ReleaseSRWLockExclusive(&rwlock->lock);
    } else {
        ReleaseSRWLockShared(&rwlock->lock);
    }

    return 0;
}

uint32 pcthread_get_num_procs()
{
    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);

    return sysinfo.dwNumberOfProcessors;
}

#define pthread_exit(a) {return (a);}

#endif