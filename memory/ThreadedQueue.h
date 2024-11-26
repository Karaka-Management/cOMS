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

#include "RingMemory.h"

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
    #include "../platform/win32/threading/Semaphore.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
    #include "../platform/linux/threading/Semaphore.h"
#endif

typedef RingMemory ThreadedQueue;

inline
void threaded_queue_alloc(ThreadedQueue* queue, uint64 size, uint32 element_count, int32 alignment = 64)
{
    ring_alloc(queue, size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_init(ThreadedQueue* queue, BufferMemory* buf, uint64 size, uint32 element_count, int32 alignment = 64)
{
    ring_init(queue, buf, size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_init(ThreadedQueue* queue, byte* buf, uint64 size, uint32 element_count, int32 alignment = 64)
{
    ring_init(queue, buf, size, alignment);

    pthread_mutex_init(&queue->mutex, NULL);
    pthread_cond_init(&queue->cond, NULL);

    sem_init(&queue->empty, element_count);
    sem_init(&queue->full, 0);
}

inline
void threaded_queue_free(ThreadedQueue* queue)
{
    ring_free(queue);
    sem_destroy(&queue->empty);
    sem_destroy(&queue->full);
    pthread_mutex_destroy(&queue->mutex);
    pthread_cond_destroy(&queue->cond);
}

// Conditional Lock
inline
void threaded_queue_enqueue(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe(queue, size)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    byte* mem = ring_get_memory(queue, size, aligned);
    memcpy(mem, data, size);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* threaded_queue_enqueue_start(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (!ring_commit_safe(queue, size, aligned)) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    return ring_get_memory(queue, size, aligned);
}

inline
void threaded_queue_enqueue_end(ThreadedQueue* queue)
{
    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

inline
byte* threaded_queue_dequeue(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&queue->mutex);

    while (queue->head == queue->tail) {
        pthread_cond_wait(&queue->cond, &queue->mutex);
    }

    memcpy(data, queue->tail, size);
    ring_move_pointer(queue, &queue->tail, size, aligned);

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
    ring_move_pointer(queue, &queue->tail, size, aligned);

    pthread_cond_signal(&queue->cond);
    pthread_mutex_unlock(&queue->mutex);
}

// Semaphore Lock
inline
void threaded_queue_enqueue_sem(ThreadedQueue* queue, byte* data, uint64 size, byte aligned = 0)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    byte* mem = ring_get_memory(queue, size, aligned);
    memcpy(mem, data, size);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->full);
}

inline
byte* threaded_queue_enqueue_start_sem(ThreadedQueue* queue, uint64 size, byte aligned = 0)
{
    sem_wait(&queue->empty);
    pthread_mutex_lock(&queue->mutex);

    return ring_get_memory(queue, size, aligned);
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
    ring_move_pointer(queue, &queue->tail, size, aligned);

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
    ring_move_pointer(queue, &queue->tail, size, aligned);

    pthread_mutex_unlock(&queue->mutex);
    sem_post(&queue->empty);
}

#endif