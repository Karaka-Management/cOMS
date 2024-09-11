/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_HASHMAP_H
#define TOS_STDLIB_HASHMAP_H

#include "../hash/GeneralHash.h"
#include "../memory/RingMemory.h"
#include "../memory/BufferMemory.h"
#include "../memory/ChunkMemory.h"
#include "Types.h"

#define MAX_KEY_LENGTH 32

struct HashEntryInt32 {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryInt32* next;
    int value;
};

struct HashEntryInt64 {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryInt64* next;
    int64 value;
};

struct HashEntryVoidP {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryVoidP* next;
    void* value;
};

struct HashEntryFloat {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryFloat* next;
    float value;
};

struct HashEntryStr {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryStr* next;
    char value[MAX_KEY_LENGTH];
};

struct HashEntry {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntry* next;
    byte* value;
};

struct HashMap {
    void** table;
    ChunkMemory buf;
};

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int count, int element_size, RingMemory* ring)
{
    hm->table = (void **) ring_get_memory(ring, count * sizeof(void *));

    hm->buf.memory = ring_get_memory(ring, count * element_size);
    hm->buf.free = (uint64 *) ring_get_memory(ring, CEIL_DIV(count, 64) * sizeof(hm->buf.free));
    hm->buf.count = count;
    hm->buf.chunk_size = element_size;
    hm->buf.last_pos = -1;
    hm->buf.alignment = 1;
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int count, int element_size, BufferMemory* buf)
{
    hm->table = (void **) buffer_get_memory(buf, count * sizeof(void *));

    hm->buf.memory = buffer_get_memory(buf, count * element_size);
    hm->buf.free = (uint64 *) buffer_get_memory(buf, CEIL_DIV(count, 64) * sizeof(hm->buf.free));
    hm->buf.count = count;
    hm->buf.chunk_size = element_size;
    hm->buf.last_pos = -1;
    hm->buf.alignment = 1;
}

inline
int64 hashmap_get_buffer_size(int count, int element_size)
{
    return sizeof(void *) * count // table
        + count * element_size // elements
        + sizeof(uint64) * CEIL_DIV(count, 64); // free
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int count, int element_size, byte* buf)
{
    hm->table = (void **) buf;

    hm->buf.memory = buf + sizeof(void *) * count;
    hm->buf.free = (uint64 *) (hm->buf.memory + count * element_size);
    hm->buf.count = count;
    hm->buf.chunk_size = element_size;
    hm->buf.last_pos = -1;
    hm->buf.alignment = 1;
}

void hashmap_insert(HashMap* hm, const char* key, int32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32* entry = (HashEntryInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = (HashEntryInt32 *) hm->table[index];
    hm->table[index] = entry;
}

void hashmap_insert(HashMap* hm, const char* key, int64 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64* entry = (HashEntryInt64 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = (HashEntryInt64 *) hm->table[index];
    hm->table[index] = entry;
}

void hashmap_insert(HashMap* hm, const char* key, void* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidP* entry = (HashEntryVoidP *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = (HashEntryVoidP *) hm->table[index];
    hm->table[index] = entry;
}

void hashmap_insert(HashMap* hm, const char* key, float value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloat* entry = (HashEntryFloat *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = (HashEntryFloat *) hm->table[index];
    hm->table[index] = entry;
}

void hashmap_insert(HashMap* hm, const char* key, const char* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryStr* entry = (HashEntryStr *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    strncpy(entry->value, value, MAX_KEY_LENGTH);
    entry->value[MAX_KEY_LENGTH - 1] = '\0';

    entry->next = (HashEntryStr *) hm->table[index];
    hm->table[index] = entry;
}

void hashmap_insert(HashMap* hm, const char* key, byte* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->value = (byte *) entry + sizeof(HashEntry);

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    memcpy(entry->value, value, hm->buf.chunk_size - sizeof(HashEntry));

    entry->next = (HashEntry *) hm->table[index];
    hm->table[index] = entry;
}

void* hashmap_get_entry(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) hm->table[index];

    while (entry != NULL) {
        if (strncmp(entry->key, key, MAX_KEY_LENGTH) == 0) {
            return entry;
        }

        entry = (HashEntry *) entry->next;
    }

    return NULL;
}

void hashmap_delete_entry(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key);
    HashEntry* entry = (HashEntry *) hm->table[index];
    HashEntry* prev = NULL;

    while (entry != NULL) {
        if (strncmp(entry->key, key, MAX_KEY_LENGTH) == 0) {
            if (prev == NULL) {
                hm->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            chunk_free_element(&hm->buf, entry->element_id);

            return;
        }

        prev = entry;
        entry = entry->next;
    }
}

#endif