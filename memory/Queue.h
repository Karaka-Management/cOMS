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
void queue_alloc(Queue* queue, uint64 element_count, uint32 element_size, int32 alignment = 64)
{
    ring_alloc(queue, element_count * element_size, alignment);
}

inline
void queue_init(Queue* queue, BufferMemory* buf, uint64 element_count, uint32 element_size, int32 alignment = 64)
{
    ring_init(queue, buf, element_count * element_size, alignment);
}

inline
void queue_init(Queue* queue, byte* buf, uint64 element_count, uint32 element_size, int32 alignment = 64)
{
    ring_init(queue, buf, element_count * element_size, alignment);
}

inline
void queue_free(Queue* queue)
{
    ring_free(queue);
}

inline
bool queue_is_empty(Queue* queue) {
    return queue->head == queue->tail;
}

inline
bool queue_set_empty(Queue* queue) {
    return queue->head = queue->tail;
}

inline
bool queue_is_full(Queue* queue, uint64 size, byte aligned = 0) {
    return !ring_commit_safe((RingMemory *) queue, size, aligned);
}

// Conditional Lock
inline
byte* queue_enqueue(Queue* queue, byte* data, uint64 size, byte aligned = 0)
{
    byte* mem = ring_get_memory_nomove(queue, size, aligned);
    memcpy(mem, data, size);
    ring_move_pointer(queue, &queue->head, size, aligned);

    return mem;
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
bool queue_dequeue(Queue* queue, byte* data, uint64 size, byte aligned = 0)
{
    if (queue->head == queue->tail) {
        return false;
    }

    if (size == 4) {
        *((int32 *) data) = *((int32 *) queue->tail);
    } else {
        memcpy(data, queue->tail, size);
    }

    ring_move_pointer(queue, &queue->tail, size, aligned);

    return true;
}

inline
byte* queue_dequeue_keep(Queue* queue, uint64 size, byte aligned = 0)
{
    if (queue->head == queue->tail) {
        return NULL;
    }

    byte* data = queue->tail;
    ring_move_pointer(queue, &queue->tail, size, aligned);

    return data;
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