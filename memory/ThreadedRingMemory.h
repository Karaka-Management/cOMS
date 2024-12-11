/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MEMORY_THREADED_RING_MEMORY_H
#define TOS_MEMORY_THREADED_RING_MEMORY_H

#include "RingMemory.h"

#if _WIN32
    #include "../platform/win32/threading/Thread.h"
#elif __linux__
    #include "../platform/linux/threading/Thread.h"
#endif

// @todo This is a horrible implementation. Please implement a lock free solution

struct ThreadedRingMemory {
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

    pthread_mutex_t mutex;
};

// @bug alignment should also include the end point, not just the start

inline
void thrd_ring_alloc(ThreadedRingMemory* ring, uint64 size, int32 alignment = 64)
{
    ring_alloc((RingMemory *) ring, size, alignment);
    pthread_mutex_init(&ring->mutex, NULL);
}

inline
void thrd_ring_init(ThreadedRingMemory* ring, BufferMemory* buf, uint64 size, int32 alignment = 64)
{
    ring_init((RingMemory *) ring, buf, size, alignment);
    pthread_mutex_init(&ring->mutex, NULL);
}

inline
void thrd_ring_init(ThreadedRingMemory* ring, byte* buf, uint64 size, int32 alignment = 64)
{
    ring_init((RingMemory *) ring, buf, size, alignment);
    pthread_mutex_init(&ring->mutex, NULL);
}

inline
void thrd_ring_free(ThreadedRingMemory* ring)
{
    ring_free((RingMemory *) ring);
    pthread_mutex_destroy(&ring->mutex);
}

inline
byte* thrd_ring_calculate_position(ThreadedRingMemory* ring, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&ring->mutex);
    byte* result = ring_calculate_position((RingMemory *) ring, size, aligned);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

inline
void thrd_ring_reset(ThreadedRingMemory* ring)
{
    pthread_mutex_lock(&ring->mutex);
    ring_reset((RingMemory *) ring);
    pthread_mutex_unlock(&ring->mutex);
}

// Moves a pointer based on the size you want to consume (new position = after consuming size)
void thrd_ring_move_pointer(ThreadedRingMemory* ring, byte** pos, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&ring->mutex);
    ring_move_pointer((RingMemory *) ring, pos, size, aligned);
    pthread_mutex_unlock(&ring->mutex);
}

byte* thrd_ring_get_memory(ThreadedRingMemory* ring, uint64 size, byte aligned = 0, bool zeroed = false)
{
    pthread_mutex_lock(&ring->mutex);
    byte* result = ring_get_memory((RingMemory *) ring, size, aligned, zeroed);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

// Same as ring_get_memory but DOESN'T move the head
byte* thrd_ring_get_memory_nomove(ThreadedRingMemory* ring, uint64 size, byte aligned = 0, bool zeroed = false)
{
    pthread_mutex_lock(&ring->mutex);
    byte* result = ring_get_memory_nomove((RingMemory *) ring, size, aligned, zeroed);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

// Used if the ring only contains elements of a certain size
// This way you can get a certain element
inline
byte* thrd_ring_get_element(ThreadedRingMemory* ring, uint64 element_count, uint64 element, uint64 size)
{
    pthread_mutex_lock(&ring->mutex);
    byte* result = ring_get_element((RingMemory *) ring, element_count, element, size);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

/**
 * Checks if one additional element can be inserted without overwriting the tail index
 */
inline
bool thrd_ring_commit_safe(ThreadedRingMemory* ring, uint64 size, byte aligned = 0)
{
    pthread_mutex_lock(&ring->mutex);
    bool result = ring_commit_safe((RingMemory *) ring, size, aligned);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

inline
void thrd_ring_force_head_update(const ThreadedRingMemory* ring)
{
    _mm_clflush(ring->head);
}

inline
void thrd_ring_force_tail_update(const ThreadedRingMemory* ring)
{
    _mm_clflush(ring->tail);
}

inline
int64 thrd_ring_dump(ThreadedRingMemory* ring, byte* data)
{
    pthread_mutex_lock(&ring->mutex);
    int64 result = ring_dump((RingMemory *) ring, data);
    pthread_mutex_unlock(&ring->mutex);

    return result;
}

#endif