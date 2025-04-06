/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_THREADS_THREAD_POOL_H
#define COMS_THREADS_THREAD_POOL_H

#include "../stdlib/Types.h"
#include "../memory/Queue.h"
#include "../memory/BufferMemory.h"
#include "../log/DebugMemory.h"
#include "Thread.h"
#include "Atomic.h"
#include "ThreadJob.h"

struct ThreadPool {
    // This is not a threaded queue since we want to handle the mutex in here, not in the queue for finer control
    Queue work_queue;

    mutex work_mutex;
    mutex_cond work_cond;
    mutex_cond working_cond;

    alignas(4) atomic_32 int32 working_cnt;
    alignas(4) atomic_32 int32 thread_cnt;

    int32 size;
    alignas(4) atomic_32 int32 state;

    alignas(4) atomic_32 int32 id_counter;
};

static THREAD_RETURN thread_pool_worker(void* arg)
{
    ThreadPool* pool = (ThreadPool *) arg;
    PoolWorker* work;

    while (true) {
        mutex_lock(&pool->work_mutex);
        while (queue_is_empty(&pool->work_queue) && !pool->state) {
            coms_pthread_cond_wait(&pool->work_cond, &pool->work_mutex);
        }

        if (pool->state == 1) {
            mutex_unlock(&pool->work_mutex);

            break;
        }

        work = (PoolWorker *) queue_dequeue_keep(&pool->work_queue);
        mutex_unlock(&pool->work_mutex);

        if (!work) {
            continue;
        }

        atomic_increment_relaxed(&pool->working_cnt);
        atomic_set_release(&work->state, 2);
        LOG_2("ThreadPool worker started");
        work->func(work);
        LOG_2("ThreadPool worker ended");
        // At the end of a thread the ring memory automatically is considered freed
        DEBUG_MEMORY_FREE((uintptr_t) work->ring.memory);
        LOG_2("Freed thread RingMemory: %n B", {{LOG_DATA_UINT64, &work->ring.size}});
        atomic_set_release(&work->state, 1);

        // Job gets marked after completion -> can be overwritten now
        if (atomic_get_relaxed(&work->id) == -1) {
            atomic_set_release(&work->id, 0);
        }

        atomic_decrement_relaxed(&pool->working_cnt);

        if (atomic_get_relaxed(&pool->state) == 0 && atomic_get_relaxed(&pool->working_cnt) == 0) {
            coms_pthread_cond_signal(&pool->working_cond);
        }
    }

    coms_pthread_cond_signal(&pool->working_cond);
    atomic_decrement_relaxed(&pool->thread_cnt);

    return (THREAD_RETURN) NULL;
}

void thread_pool_alloc(ThreadPool* pool, int32 thread_count, int32 worker_count, int32 alignment = 64)
{
    PROFILE(PROFILE_THREAD_POOL_ALLOC);
    LOG_1(
        "Allocating thread pool with %d threads and %d queue length",
        {
            {LOG_DATA_INT32, &thread_count},
            {LOG_DATA_INT32, &worker_count}
        }
    );

    queue_alloc(&pool->work_queue, worker_count, sizeof(PoolWorker), alignment);

    pool->thread_cnt = thread_count;

    // @todo switch from pool mutex and pool cond to threadjob mutex/cond
    //      thread_pool_wait etc. should just iterate over all mutexes
    mutex_init(&pool->work_mutex, NULL);
    coms_pthread_cond_init(&pool->work_cond, NULL);
    coms_pthread_cond_init(&pool->working_cond, NULL);

    coms_pthread_t thread;
    for (pool->size = 0; pool->size < thread_count; ++pool->size) {
        coms_pthread_create(&thread, NULL, thread_pool_worker, pool);
        coms_pthread_detach(thread);
    }
}

void thread_pool_create(ThreadPool* pool, BufferMemory* buf, int32 thread_count, int32 worker_count, int32 alignment = 64)
{
    PROFILE(PROFILE_THREAD_POOL_ALLOC);
    LOG_1(
        "Creating thread pool with %d threads and %d queue length",
        {
            {LOG_DATA_INT32, &thread_count},
            {LOG_DATA_INT32, &worker_count}
        }
    );

    queue_init(&pool->work_queue, buf, worker_count, sizeof(PoolWorker), alignment);

    pool->thread_cnt = thread_count;

    // @todo switch from pool mutex and pool cond to threadjob mutex/cond
    //      thread_pool_wait etc. should just iterate over all mutexes
    mutex_init(&pool->work_mutex, NULL);
    coms_pthread_cond_init(&pool->work_cond, NULL);
    coms_pthread_cond_init(&pool->working_cond, NULL);

    coms_pthread_t thread;
    for (pool->size = 0; pool->size < thread_count; ++pool->size) {
        coms_pthread_create(&thread, NULL, thread_pool_worker, pool);
        coms_pthread_detach(thread);
    }
}

void thread_pool_wait(ThreadPool* pool)
{
    mutex_lock(&pool->work_mutex);
    while ((!pool->state && pool->working_cnt != 0) || (pool->state && pool->thread_cnt != 0)) {
        coms_pthread_cond_wait(&pool->working_cond, &pool->work_mutex);
    }
    mutex_unlock(&pool->work_mutex);
}

void thread_pool_destroy(ThreadPool* pool)
{
    // This sets the queue to empty
    atomic_set_release((void **) &pool->work_queue.tail, pool->work_queue.head);

    // This sets the state to "shutdown"
    atomic_set_release(&pool->state, 1);

    coms_pthread_cond_broadcast(&pool->work_cond);
    thread_pool_wait(pool);

    mutex_destroy(&pool->work_mutex);
    coms_pthread_cond_destroy(&pool->work_cond);
    coms_pthread_cond_destroy(&pool->working_cond);
}

PoolWorker* thread_pool_add_work(ThreadPool* pool, const PoolWorker* job)
{
    mutex_lock(&pool->work_mutex);
    PoolWorker* temp_job = (PoolWorker *) ring_get_memory_nomove((RingMemory *) &pool->work_queue, sizeof(PoolWorker), 8);
    if (atomic_get_relaxed(&temp_job->id) > 0) {
        mutex_unlock(&pool->work_mutex);
        ASSERT_SIMPLE(temp_job->id == 0);

        return NULL;
    }

    memcpy(temp_job, job, sizeof(PoolWorker));
    ring_move_pointer((RingMemory *) &pool->work_queue, &pool->work_queue.head, sizeof(PoolWorker), 8);

    if (temp_job->id == 0) {
        temp_job->id = atomic_fetch_add_acquire(&pool->id_counter, 1);
    }

    coms_pthread_cond_broadcast(&pool->work_cond);
    mutex_unlock(&pool->work_mutex);

    return temp_job;
}

// This is basically the same as thread_pool_add_work but allows us to directly write into the memory in the caller
// This makes it faster, since we can avoid a memcpy
PoolWorker* thread_pool_add_work_start(ThreadPool* pool)
{
    mutex_lock(&pool->work_mutex);

    PoolWorker* temp_job = (PoolWorker *) queue_enqueue_start(&pool->work_queue);
    if (atomic_get_relaxed(&temp_job->id) > 0) {
        mutex_unlock(&pool->work_mutex);
        ASSERT_SIMPLE(temp_job->id == 0);

        return NULL;
    }

    if (temp_job->id == 0) {
        // +1 because otherwise the very first job would be id = 0 which is not a valid id
        temp_job->id = atomic_fetch_add_acquire(&pool->id_counter, 1) + 1;
    }

    return temp_job;
}

void thread_pool_add_work_end(ThreadPool* pool)
{
    queue_enqueue_end(&pool->work_queue);
    coms_pthread_cond_broadcast(&pool->work_cond);
    mutex_unlock(&pool->work_mutex);
}


#endif