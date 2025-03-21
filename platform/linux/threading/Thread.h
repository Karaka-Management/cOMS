/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_PLATFORM_LINUX_THREADING_THREAD_H
#define COMS_PLATFORM_LINUX_THREADING_THREAD_H

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

inline int32 futex_wait(int32 *futex, int32 val) {
    return syscall(SYS_futex, futex, FUTEX_WAIT, val, NULL, NULL, 0);
}

inline int32 futex_wake(int32 *futex, int32 n) {
    return syscall(SYS_futex, futex, FUTEX_WAKE, n, NULL, NULL, 0);
}

inline int32 coms_pthread_create(coms_pthread_t* thread, void*, ThreadJobFunc start_routine, void* arg) {
    if (thread == NULL || start_routine == NULL) {
        return 1;
    }

    int32 flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM;
    *thread = clone((int32 (*)(void*))start_routine, (void*)((char*)malloc(4096) + 4096), flags, arg);
    if (*thread == -1) {
        return 1;
    }

    return 0;
}

inline int32 coms_pthread_join(coms_pthread_t thread, void** retval) {
    if (syscall(SYS_waitid, P_PID, thread, retval, WEXITED, NULL) == -1) {
        return 1;
    }
    return 0;
}

inline int32 coms_pthread_detach(coms_pthread_t) {
    // In Linux, threads are automatically detached when they exit.
    return 0;
}

inline int32 coms_pthread_mutex_init(coms_pthread_mutex_t* mutex, coms_pthread_mutexattr_t*) {
    if (mutex == NULL) {
        return 1;
    }
    mutex->futex = 0;
    return 0;
}

inline int32 coms_pthread_mutex_destroy(coms_pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return 1;
    }
    return 0;
}

inline int32 coms_pthread_mutex_lock(coms_pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return 1;
    }
    while (__atomic_exchange_n(&mutex->futex, 1, __ATOMIC_ACQUIRE) != 0) {
        futex_wait(&mutex->futex, 1);
    }
    return 0;
}

inline int32 coms_pthread_mutex_unlock(coms_pthread_mutex_t* mutex) {
    if (mutex == NULL) {
        return 1;
    }
    __atomic_store_n(&mutex->futex, 0, __ATOMIC_RELEASE);
    futex_wake(&mutex->futex, 1);
    return 0;
}

inline int32 coms_pthread_cond_init(coms_pthread_cond_t* cond, coms_pthread_condattr_t*) {
    if (cond == NULL) {
        return 1;
    }
    cond->futex = 0;
    return 0;
}

inline int32 coms_pthread_cond_destroy(coms_pthread_cond_t* cond) {
    if (cond == NULL) {
        return 1;
    }
    return 0;
}

inline int32 coms_pthread_cond_timedwait(coms_pthread_cond_t* cond, coms_pthread_mutex_t* mutex, const struct timespec*) {
    if (cond == NULL || mutex == NULL) {
        return 1;
    }
    int32 oldval = __atomic_load_n(&cond->futex, __ATOMIC_ACQUIRE);
    coms_pthread_mutex_unlock(mutex);
    futex_wait(&cond->futex, oldval);
    coms_pthread_mutex_lock(mutex);
    return 0;
}

inline int32 coms_pthread_cond_wait(coms_pthread_cond_t* cond, coms_pthread_mutex_t* mutex) {
    return coms_pthread_cond_timedwait(cond, mutex, NULL);
}

inline int32 coms_pthread_cond_signal(coms_pthread_cond_t* cond) {
    if (cond == NULL) {
        return 1;
    }
    __atomic_add_fetch(&cond->futex, 1, __ATOMIC_RELEASE);
    futex_wake(&cond->futex, 1);
    return 0;
}

inline int32 coms_pthread_cond_broadcast(coms_pthread_cond_t* cond) {
    if (cond == NULL) {
        return 1;
    }
    __atomic_add_fetch(&cond->futex, 1, __ATOMIC_RELEASE);
    futex_wake(&cond->futex, INT32_MAX);
    return 0;
}

inline int32 coms_pthread_rwlock_init(coms_pthread_rwlock_t* rwlock, const coms_pthread_rwlockattr_t*) {
    if (rwlock == NULL) {
        return 1;
    }
    rwlock->futex = 0;
    rwlock->exclusive = false;
    return 0;
}

inline int32 coms_pthread_rwlock_destroy(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    return 0;
}

inline int32 coms_pthread_rwlock_rdlock(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    while (1) {
        int32 val = __atomic_load_n(&rwlock->futex, __ATOMIC_ACQUIRE);
        if (val >= 0 && __atomic_compare_exchange_n(&rwlock->futex, &val, val + 1, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
            break;
        }
        futex_wait(&rwlock->futex, val);
    }
    return 0;
}

inline int32 coms_pthread_rwlock_tryrdlock(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    int32 val = __atomic_load_n(&rwlock->futex, __ATOMIC_ACQUIRE);
    if (val >= 0 && __atomic_compare_exchange_n(&rwlock->futex, &val, val + 1, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
        return 0;
    }
    return 1;
}

inline int32 coms_pthread_rwlock_wrlock(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    while (1) {
        int32 val = __atomic_load_n(&rwlock->futex, __ATOMIC_ACQUIRE);
        if (val == 0 && __atomic_compare_exchange_n(&rwlock->futex, &val, -1, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
            rwlock->exclusive = true;
            break;
        }
        futex_wait(&rwlock->futex, val);
    }
    return 0;
}

inline int32 coms_pthread_rwlock_trywrlock(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    int32 val = __atomic_load_n(&rwlock->futex, __ATOMIC_ACQUIRE);
    if (val == 0 && __atomic_compare_exchange_n(&rwlock->futex, &val, -1, false, __ATOMIC_ACQ_REL, __ATOMIC_ACQUIRE)) {
        rwlock->exclusive = true;
        return 0;
    }
    return 1;
}

inline int32 coms_pthread_rwlock_unlock(coms_pthread_rwlock_t* rwlock) {
    if (rwlock == NULL) {
        return 1;
    }
    if (rwlock->exclusive) {
        rwlock->exclusive = false;
        __atomic_store_n(&rwlock->futex, 0, __ATOMIC_RELEASE);
        futex_wake(&rwlock->futex, 1);
    } else {
        int32 val = __atomic_sub_fetch(&rwlock->futex, 1, __ATOMIC_RELEASE);
        if (val == 0) {
            futex_wake(&rwlock->futex, 1);
        }
    }
    return 0;
}

inline uint32 pcthread_get_num_procs() {
    return sysconf(_SC_NPROCESSORS_ONLN);
}

#define coms_pthread_exit(a) { return (a); }

#endif