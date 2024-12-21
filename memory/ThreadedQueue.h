/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_THREADED_QUEUE_H
#define TOS_MEMORY_THREADED_QUEUE_H

// @todo This is a horrible implementation. Please implement a lock free solution

#include "../stdlib/Types.h"
#include "../utils/Utils.h"
#include "RingMemory.h"

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
    #include "../platform/win32/threading/Semaphore.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
    #include "../platform/linux/threading/Semaphore.h"
#endif

struct ThreadedQueue {
    byte* memory;
    byte* end;

    byte* head;

    // This variable is usually only used by single producer/consumer code mostly found in threads.
    // One thread inserts elements -> updates head
    // The other thread reads elements -> updates tail
    // This code itself doesn't change this variable
    byte* tail;

    uint64 size;
    uint32 alignment;

    // The ring memory ends here
    uint32 element_size;

    // We support both conditional locking and semaphore locking
    // These values are not initialized and not used unless you use the queue
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    sem_t empty;
    sem_t full;
};

inline
void thrd_queue_alloc(ThreadedQueue* queue, uint32 element_count, uint32 element_size, uint32 alignment = 64)
{
    element_size = ROUND_TO_NEAREST(element_size, alignment);

    ring_alloc((RingMemory *) queue, element_count * element_size, alignment);

    queue->element_size = element_size;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void thrd_queue_init(ThreadedQueue* queue, BufferMemory* buf, uint32 element_count, uint32 element_size, uint32 alignment = 64)
{
    element_size = ROUND_TO_NEAREST(element_size, alignment);

    ring_init((RingMemory *) queue, buf, element_count * element_size, alignment);

    queue->element_size = element_size;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void thrd_queue_init(ThreadedQueue* queue, byte* buf, uint32 element_count, uint32 element_size, uint32 alignment = 64)
{
    element_size = ROUND_TO_NEAREST(element_size, alignment);

    ring_init((RingMemory *) queue, buf, element_count * element_size, alignment);

    queue->element_size = element_size;

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void thrd_queue_free(ThreadedQueue* queue)
{
    ring_free((RingMemory *) queue);
    sem_destroy(&queue->empty);
    sem_destroy(&queue->full);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

// @todo Create enqueue_unique and enqueue_unique_sem
inline
void thrd_queue_enqueue_unique_wait(ThreadedQueue* queue, const byte* data)
{
    ASSERT_SIMPLE((uint64_t) data % 4 == 0);
    pthread_mutex_lock(&queue->mutex);

    byte* tail = queue->tail;
    while (tail != queue->tail) {
        ASSERT_SIMPLE((uint64_t) tail % 4 == 0);

        // @performance we could probably make this faster since we don't need to compare the entire range
        if (is_equal_aligned(tail, data, queue->element_size) == 0) {
            pthread_mutex_unlock(&queue->mutex);

            return;
        }

        ring_move_pointer((RingMemory *) queue, &tail, queue->element_size, queue->alignment);
    }

    while (!ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
void thrd_queue_enqueue_unique(ThreadedQueue* queue, const byte* data)
{
    ASSERT_SIMPLE((uint64_t) data % 4 == 0);
    pthread_mutex_lock(&queue->mutex);

    byte* tail = queue->tail;
    while (tail != queue->tail) {
        ASSERT_SIMPLE((uint64_t) tail % 4 == 0);

        // @performance we could probably make this faster since we don't need to compare the entire range
        if (is_equal_aligned(tail, data, queue->element_size) == 0) {
            pthread_mutex_unlock(&queue->mutex);

            return;
        }

        ring_move_pointer((RingMemory *) queue, &tail, queue->element_size, queue->alignment);
    }

    if (!ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment)) {
        pthread_mutex_unlock(&queue->mutex);

        return;
    }

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// Conditional Lock
inline
void thrd_queue_enqueue(ThreadedQueue* queue, const byte* data)
{
    pthread_mutex_lock(&queue->mutex);

    if (!ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment)) {
        pthread_mutex_unlock(&queue->mutex);

        return;
    }

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
void thrd_queue_enqueue_wait(ThreadedQueue* queue, const byte* data)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* thrd_queue_enqueue_start_wait(ThreadedQueue* queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    return ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
}

inline
void thrd_queue_enqueue_end_wait(ThreadedQueue* queue)
{
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
bool thrd_queue_dequeue(ThreadedQueue* queue, byte* data)
{
    if (queue->head == queue->tail) {
        return false;
    }

    // we do this twice because the first one is very fast but may return a false positive
    pthread_mutex_lock(&queue->mutex);
    if (queue->head == queue->tail) {
        pthread_mutex_unlock(&queue->mutex);

        return false;
    }

    if (queue->element_size == 4) {
        *((int32 *) data) = *((int32 *) queue->tail);
    } else {
        memcpy(data, queue->tail, queue->element_size);
    }
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);

    return true;
}

inline
bool thrd_queue_empty(ThreadedQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    bool is_empty = queue->head == queue->tail;
    pthread_mutex_unlock(&queue->mutex);

    return is_empty;
}

inline
bool thrd_queue_full(ThreadedQueue* queue) {
    pthread_mutex_lock(&queue->mutex);
    bool is_full = !ring_commit_safe((RingMemory *) queue, queue->element_size, queue->alignment);
    pthread_mutex_unlock(&queue->mutex);

    return is_full;
}

// Waits until a dequeue is available
inline
void thrd_queue_dequeue_wait(ThreadedQueue* queue, byte* data)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == queue->tail) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    memcpy(data, queue->tail, queue->element_size);
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* thrd_queue_dequeue_start_wait(ThreadedQueue* queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == queue->tail) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    return queue->tail;
}

inline
void thrd_queue_dequeue_end_wait(ThreadedQueue* queue)
{
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// Semaphore Lock
inline
void thrd_queue_enqueue_sem_wait(ThreadedQueue* queue, const byte* data)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
}

inline
bool thrd_queue_enqueue_sem_timedwait(ThreadedQueue* queue, const byte* data, uint64 wait)
{
    if (sem_timedwait(&queue->empty, wait)) {
        return false;
    }

    pthread_mutex_lock(&queue->mutex);

    byte* mem = ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
    memcpy(mem, data, queue->element_size);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);

    return true;
}

inline
byte* thrd_queue_enqueue_start_sem_wait(ThreadedQueue* queue)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    return ring_get_memory((RingMemory *) queue, queue->element_size, queue->alignment);
}

inline
void thrd_queue_enqueue_end_sem_wait(ThreadedQueue* queue)
{
    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
}

inline
byte* thrd_queue_dequeue_sem_wait(ThreadedQueue* queue, byte* data)
{
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);

    memcpy(data, queue->tail, queue->element_size);
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
}

inline
bool thrd_queue_dequeue_sem_timedwait(ThreadedQueue* queue, byte* data, uint64 wait)
{
    if (sem_timedwait(&queue->full, wait)) {
        return false;
    }

    pthread_mutex_lock(&queue->mutex);

    memcpy(data, queue->tail, queue->element_size);
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);

    return true;
}

inline
byte* thrd_queue_dequeue_start_sem_wait(ThreadedQueue* queue)
{
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);

    return queue->tail;
}

inline
void thrd_queue_dequeue_end_sem_wait(ThreadedQueue* queue)
{
    ring_move_pointer((RingMemory *) queue, &queue->tail, queue->element_size, queue->alignment);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
}

#endif