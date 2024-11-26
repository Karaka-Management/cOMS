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
#include "RingMemory.h"

typedef RingMemory Queue;

inline
void queue_alloc(Queue* queue, uint64 size, uint32 element_size, int32 alignment = 64)
{
    ring_alloc(queue, size, alignment);
}

inline
void queue_init(Queue* queue, BufferMemory* buf, uint64 size, uint32 element_size, int32 alignment = 64)
{
    ring_init(queue, buf, size, alignment);
}

inline
void queue_init(Queue* queue, byte* buf, uint64 size, uint32 element_size, int32 alignment = 64)
{
    ring_init(queue, buf, size, alignment);
}

inline
void queue_free(Queue* queue)
{
    ring_free(queue);
}

// Conditional Lock
inline
void queue_enqueue(Queue* queue, byte* data, uint64 size, byte aligned = 0)
{
    byte* mem = ring_get_memory_nomove(queue, size, aligned);
    memcpy(mem, data, size);
    ring_move_pointer(queue, &queue->head, size, aligned);
}

inline
byte* queue_enqueue_start(Queue* queue, uint64 size, byte aligned = 0)
{
    return ring_get_memory_nomove(queue, size, aligned);
}

inline
void queue_enqueue_end(Queue* queue, uint64 size, byte aligned = 0)
{
    ring_move_pointer(queue, &queue->head, size, aligned);
}

inline
byte* queue_dequeue(Queue* queue, byte* data, uint64 size, byte aligned = 0)
{
    memcpy(data, queue->tail, size);
    ring_move_pointer(queue, &queue->tail, size, aligned);
}

inline
byte* queue_dequeue_start(Queue* queue)
{
    return queue->tail;
}

inline
void queue_dequeue_end(Queue* queue, uint64 size, byte aligned = 0)
{
    ring_move_pointer(queue, &queue->tail, size, aligned);
}

#endif