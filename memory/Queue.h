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

typedef RingMemory Queue;

inline
void queue_alloc(Queue* ring, uint64 size, int32 alignment = 64)
{
    ring_alloc(ring, size, alignment);

    pthread_mutex_init(&ring->mutex, NULL);
    pthread_cond_init(&ring->cond, NULL);
}

inline
void queue_init(Queue* ring, BufferMemory* buf, uint64 size, int32 alignment = 64)
{
    ring_init(ring, buf, size, alignment);

    pthread_mutex_init(&ring->mutex, NULL);
    pthread_cond_init(&ring->cond, NULL);
}

inline
void queue_free(Queue* buf)
{
    ring_free(buf);
}

inline
void queue_init(Queue* ring, byte* buf, uint64 size, int32 alignment = 64)
{
    ring_init(ring, buf, size, alignment);
}

inline
void ring_enqueue(Queue* ring, byte* data, uint64 size)
{
    pthread_mutex_lock(&ring->mutex);

    while (!ring_commit_safe(ring, size)) {
        pthread_cond_wait(&ring->cond, &ring->mutex);
    }

    byte* mem = ring_get_memory(ring, size);
    memcpy(mem, data, size);

    pthread_cond_signal(&ring->cond);
    pthread_mutex_unlock(&ring->mutex);
}

inline
byte* ring_enqueue_start(Queue* ring, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&ring->mutex);

    while (!ring_commit_safe(ring, size, aligned)) {
        pthread_cond_wait(&ring->cond, &ring->mutex);
    }

    return ring_get_memory(ring, size, aligned);
}

inline
void ring_enqueue_end(Queue* ring)
{
    pthread_cond_signal(&ring->cond);
    pthread_mutex_unlock(&ring->mutex);
}

inline
byte* ring_dequeue(Queue* ring, byte* data, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&ring->mutex);

    while (ring->head == ring->tail) {
        pthread_cond_wait(&ring->cond, &ring->mutex);
    }

    memcpy(data, ring->tail, size);
    ring_move_pointer(ring, &ring->tail, size, aligned);

    pthread_cond_signal(&ring->cond);
    pthread_mutex_unlock(&ring->mutex);
}

inline
byte* ring_dequeue_start(Queue* ring)
{
    pthread_mutex_lock(&ring->mutex);

    while (ring->head == ring->tail) {
        pthread_cond_wait(&ring->cond, &ring->mutex);
    }

    return ring->tail;
}

inline
void ring_dequeue_end(Queue* ring, uint64 size, byte aligned = 0)
{
    ring_move_pointer(ring, &ring->tail, size, aligned);

    pthread_cond_signal(&ring->cond);
    pthread_mutex_unlock(&ring->mutex);
}

#endif