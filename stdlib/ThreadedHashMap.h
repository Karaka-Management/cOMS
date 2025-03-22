/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef COMS_STDLIB_THREADED_HASH_MAP_H
#define COMS_STDLIB_THREADED_HASH_MAP_H

#include "../stdlib/Types.h"
#include "HashMap.h"

#include "../thread/Atomic.h"
#include "../thread/Semaphore.h"
#include "../thread/Thread.h"

struct ThreadedHashMap {
    void** table;
    ChunkMemory buf;

    coms_pthread_mutex_t mutex;
};

// WARNING: element_size = element size + remaining HashEntry data size
inline
void thrd_hashmap_create(ThreadedHashMap* hm, int32 count, int32 element_size, RingMemory* ring)
{
    hashmap_create((HashMap *) hm, count, element_size, ring);
    coms_pthread_mutex_init(&hm->mutex, NULL);
}

// WARNING: element_size = element size + remaining HashEntry data size
inline
void thrd_hashmap_create(ThreadedHashMap* hm, int32 count, int32 element_size, BufferMemory* buf)
{
    hashmap_create((HashMap *) hm, count, element_size, buf);
    coms_pthread_mutex_init(&hm->mutex, NULL);
}

// WARNING: element_size = element size + remaining HashEntry data size
inline
void thrd_hashmap_create(ThreadedHashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hashmap_create((HashMap *) hm, count, element_size, buf);
    coms_pthread_mutex_init(&hm->mutex, NULL);
}

inline
void thrd_hashmap_free(ThreadedHashMap* hm)
{
    coms_pthread_mutex_destroy(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, int32 value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, int64 value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, uintptr_t value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, void* value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, f32 value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, const char* value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_insert(ThreadedHashMap* hm, const char* key, byte* value) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_insert((HashMap *) hm, key, value);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_get_entry(ThreadedHashMap* hm, HashEntry* entry, const char* key) {
    coms_pthread_mutex_lock(&hm->mutex);
    HashEntry* temp = hashmap_get_entry((HashMap *) hm, key);
    memcpy(entry, temp, hm->buf.chunk_size);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_get_entry(ThreadedHashMap* hm, HashEntry* entry, const char* key, uint64 index) {
    coms_pthread_mutex_lock(&hm->mutex);
    HashEntry* temp = hashmap_get_entry((HashMap *) hm, key, index);
    memcpy(entry, temp, hm->buf.chunk_size);
    coms_pthread_mutex_unlock(&hm->mutex);
}

inline
void thrd_hashmap_remove(ThreadedHashMap* hm, const char* key) {
    coms_pthread_mutex_lock(&hm->mutex);
    hashmap_remove((HashMap *) hm, key);
    coms_pthread_mutex_unlock(&hm->mutex);
}

#endif