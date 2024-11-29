/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_QUEUE_H
#define TOS_MEMORY_QUEUE_H

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
    int32 alignment;
    int32 element_alignment;

    // We support both conditional locking and semaphore locking
    // These values are not initialized and not used unless you use the queue
    pthread_mutex_t mutex;
    pthread_cond_t cond;

    sem_t empty;
    sem_t full;
};

inline
void threaded_queue_alloc(ThreadedQueue* queue, uint32 element_count, uint64 element_size, int32 alignment = 64)
{
    ring_alloc((RingMemory *) queue, element_count * element_size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_init(ThreadedQueue* queue, BufferMemory* buf, uint32 element_count, uint64 element_size, int32 alignment = 64)
{
    ring_init((RingMemory *) queue, buf, element_count * element_size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_init(ThreadedQueue* queue, byte* buf, uint32 element_count, uint64 element_size, int32 alignment = 64)
{
    ring_init((RingMemory *) queue, buf, element_count * element_size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_free(ThreadedQueue* queue)
{
    ring_free((RingMemory *) queue);
    sem_destroy(&queue->empty);
    sem_destroy(&queue->full);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

// @todo Create enqueue_unique
inline
void threaded_queue_enqueue_unique(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    ASSERT_SIMPLE((uint64_t) data % 4 == 0);
    pthread_mutex_lock(&queue->mutex);

    byte* tail = queue->tail;
    while (tail != queue->tail) {
        ASSERT_SIMPLE((uint64_t) tail % 4 == 0);

        // @performance we could probably make this faster since we don't need to compare the entire range
        if (is_equal_aligned(tail, data, size) == 0) {
            pthread_mutex_unlock(&queue->mutex);

            return;
        }

        ring_move_pointer((RingMemory *) queue, &tail, size, aligned);
    }

    while (!ring_commit_safe((RingMemory *) queue, size)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    byte* mem = ring_get_memory((RingMemory *) queue, size, aligned);
    memcpy(mem, data, size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// Conditional Lock
inline
void threaded_queue_enqueue(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe((RingMemory *) queue, size)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    byte* mem = ring_get_memory((RingMemory *) queue, size, aligned);
    memcpy(mem, data, size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* threaded_queue_enqueue_start(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe((RingMemory *) queue, size, aligned)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    return ring_get_memory((RingMemory *) queue, size, aligned);
}

inline
void threaded_queue_enqueue_end(ThreadedQueue* queue)
{
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
void threaded_queue_dequeue(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == queue->tail) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    memcpy(data, queue->tail, size);
    ring_move_pointer((RingMemory *) queue, &queue->tail, size, aligned);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* threaded_queue_dequeue_start(ThreadedQueue* queue)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == queue->tail) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    return queue->tail;
}

inline
void threaded_queue_dequeue_end(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    ring_move_pointer((RingMemory *) queue, &queue->tail, size, aligned);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// Semaphore Lock
inline
void threaded_queue_enqueue_sem(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    byte* mem = ring_get_memory((RingMemory *) queue, size, aligned);
    memcpy(mem, data, size);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
}

inline
byte* threaded_queue_enqueue_start_sem(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    return ring_get_memory((RingMemory *) queue, size, aligned);
}

inline
void threaded_queue_enqueue_end_sem(ThreadedQueue* queue)
{
    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
}

inline
byte* threaded_queue_dequeue_sem(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);

    memcpy(data, queue->tail, size);
    ring_move_pointer((RingMemory *) queue, &queue->tail, size, aligned);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
}

inline
byte* threaded_queue_dequeue_start_sem(ThreadedQueue* queue)
{
    sem_wait(&queue->full);
    pthread_mutex_lock(&queue->mutex);

    return queue->tail;
}

inline
void threaded_queue_dequeue_end_sem(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    ring_move_pointer((RingMemory *) queue, &queue->tail, size, aligned);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
}

#endif