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
#include "../utils/StringUtils.h"

#define MAX_KEY_LENGTH 32

struct HashEntryInt32 {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryInt32* next;
    int32 value;
};

struct HashEntryInt64 {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryInt64* next;
    int64 value;
};

struct HashEntryUIntPtr {
    int64 element_id;
    char key[MAX_KEY_LENGTH];
    HashEntryUIntPtr* next;
    uintptr_t value;
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
    f32 value;
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
void hashmap_create(HashMap* hm, int32 count, int32 element_size, RingMemory* ring)
{
    byte* data = ring_get_memory(
        ring,
        count * (sizeof(void *) + element_size)
        + CEIL_DIV(count, 64) * sizeof(hm->buf.free),
        0, true
    );

    hm->table = (void **) data;
    chunk_init(&hm->buf, data + sizeof(void *) * count, count, element_size, 1);
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, BufferMemory* buf)
{
    byte* data = buffer_get_memory(
        buf,
        count * (sizeof(void *) + element_size)
        + CEIL_DIV(count, 64) * sizeof(hm->buf.free)
    );

    hm->table = (void **) data;
    chunk_init(&hm->buf, data + sizeof(void *) * count, count, element_size, 1);
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hm->table = (void **) buf;
    chunk_init(&hm->buf, buf + sizeof(void *) * count, count, element_size, 1);
}

// Calculates how large a hashmap will be
inline
int64 hashmap_size(int count, int32 element_size)
{
    return count * sizeof(element_size) // table
        + count * element_size // elements
        + sizeof(uint64) * CEIL_DIV(count, 64); // free
}

inline
int64 hashmap_size(const HashMap* hm)
{
    return hm->buf.count * sizeof(hm->table) + hm->buf.size;
}

void hashmap_insert(HashMap* hm, const char* key, int32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32* entry = (HashEntryInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryInt32* tmp = (HashEntryInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, const char* key, int64 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64* entry = (HashEntryInt64 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryInt64* tmp = (HashEntryInt64 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, const char* key, uintptr_t value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryUIntPtr* entry = (HashEntryUIntPtr *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryUIntPtr* tmp = (HashEntryUIntPtr *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, const char* key, void* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidP* entry = (HashEntryVoidP *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryVoidP* tmp = (HashEntryVoidP *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, const char* key, f32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int64 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloat* entry = (HashEntryFloat *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, MAX_KEY_LENGTH);
    entry->key[MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryFloat* tmp = (HashEntryFloat *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
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

    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryStr* tmp = (HashEntryStr *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
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

    entry->next = NULL;

    if (hm->table[index]) {
        HashEntry* tmp = (HashEntry *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

HashEntry* hashmap_get_entry(HashMap* hm, const char* key) {
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

// This function only saves one step (omission of the hash function)
// The reason for this is in some cases we can use compile time hashing
HashEntry* hashmap_get_entry(HashMap* hm, const char* key, uint64 index) {
    index %= hm->buf.count;
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

// @bug We cannot know if the data needs endian swap (it coult be int/float, but also some other 4/8 byte value)
//  -> if we save this to a file and load it on a different system we will have "corrupt" data
inline
int64 hashmap_dump(const HashMap* hm, byte* data)
{
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(hm->buf.count);
    data += sizeof(uint64);

    uint64 next_count_total = 0;

    // Dump the table content where the elements are relative indeces/pointers
    for (int32 i = 0; i < hm->buf.count; ++i) {
        *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uintptr_t) hm->table[i] - (uintptr_t) hm->buf.memory);
        data += sizeof(uint64);

        // Also dump the next pointer
        // Count how many next elements we have
        HashEntry* entry = ((HashEntry *) hm->table[i])->next;
        int32 next_count = 0;
        while (entry) {
            ++next_count;
            entry = entry->next;
        }

        next_count_total += next_count;

        *((int32 *) data) = SWAP_ENDIAN_LITTLE(next_count);
        data += sizeof(next_count);

        if (next_count > 0) {
            entry = ((HashEntry *) hm->table[i])->next;
            while (entry) {
                *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uintptr_t) entry - (uintptr_t) hm->buf.memory);
                data += sizeof(uint64);

                entry = entry->next;
            }
        }
    }

    // @performance chunk_dump() below contains some data we already output above
    // (next pointer but it is useless, since we need relative positions)
    // Maybe we should manually re-create the chunk_dump here and omit the already dumped data for the next pointer?

    // How many bytes were written (+ dump the chunk memory)
    return sizeof(hm->buf.count)
        + hm->buf.count * sizeof(uint64) // table content
        + hm->buf.count * sizeof(int32) // counter for the next pointer (one for every element)
        + next_count_total * sizeof(uint64) // next pointer offset
        + chunk_dump(&hm->buf, data);
}

inline
int64 hashmap_load(HashMap* hm, const byte* data)
{
    uint64 count = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(uint64);

    uint64 next_count_total = 0;

    // Load the table content, we also need to convert from relative indeces to pointers
    for (int i = 0; i < count; ++i) {
        hm->table[i] = hm->buf.memory + SWAP_ENDIAN_LITTLE(*((uint64 *) data));
        data += sizeof(uint64);

        // Also load the next pointer
        // Count how many next elements we have
        int32 next_count = SWAP_ENDIAN_LITTLE(*((int32 *) data));
        data += sizeof(next_count);

        HashEntry* entry = ((HashEntry *) hm->table[i]);
        for (int32 j = 0; j < next_count; ++j) {
            entry->next = (HashEntry *) (hm->buf.memory + SWAP_ENDIAN_LITTLE(*((uint64 *) data)));
            data += sizeof(uint64);
            entry = entry->next;
        }
    }

    // How many bytes was read from data
    return sizeof(count)
        + hm->buf.count * sizeof(uint64) // table content
        + hm->buf.count * sizeof(int32) // counter for the next pointer (one for every element)
        + next_count_total * sizeof(uint64) // next pointer offset
        + chunk_load(&hm->buf, data);
}

#endif