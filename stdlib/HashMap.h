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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return;
    }

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

HashEntry* hashmap_get_entry(const HashMap* hm, const char* key) {
    // @performance Do we really want to do this check every time?
    if (hm->buf.count == 0) {
        return NULL;
    }

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
HashEntry* hashmap_get_entry(const HashMap* hm, const char* key, uint64 index) {
    if (hm->buf.count == 0) {
        return NULL;
    }

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

inline
int64 hashmap_dump(const HashMap* hm, byte* data)
{
    *((uint64 *) data) = SWAP_ENDIAN_LITTLE(hm->buf.count);
    data += sizeof(uint64);

    // Dump the table content where the elements are relative indeces/pointers
    for (int32 i = 0; i < hm->buf.count; ++i) {
        *((uint64 *) data) = hm->table[i]
            ? SWAP_ENDIAN_LITTLE((uintptr_t) hm->table[i] - (uintptr_t) hm->buf.memory)
            : 0ULL;
    }
    data += sizeof(uint64) * hm->buf.count;

    int64 value_size = hm->buf.chunk_size - sizeof(uint64) - sizeof(char) * MAX_KEY_LENGTH - sizeof(uint64);

    // Dumb hash map content = buffer memory
    int32 free_index = 0;
    int32 bit_index = 0;
    for (int32 i = 0; i < hm->buf.count; ++i) {
        if ((hm->buf.free[free_index] & (1ULL << bit_index)) > 0) {
            HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, i);

            // element_id
            *((uint64 *) data) = SWAP_ENDIAN_LITTLE(entry->element_id);
            data += sizeof(entry->element_id);

            // key
            memcpy(data, entry->key, sizeof(entry->key));
            data += sizeof(entry->key);

            // next pointer
            if (entry->next) {
                *((uint64 *) data) = SWAP_ENDIAN_LITTLE((uintptr_t) entry->next - (uintptr_t) hm->buf.memory);
            } else {
                memset(data, 0, sizeof(uint64));
            }
            data += sizeof(uint64);

            // We just assume that 4 or 8 bytes = int -> endian handling
            if (value_size == 4) {
                *((int32 *) data) = SWAP_ENDIAN_LITTLE(((HashEntryInt32 *) entry)->value);
            } else if (value_size == 8) {
                *((int64 *) data) = SWAP_ENDIAN_LITTLE(((HashEntryInt64 *) entry)->value);
            } else {
                memcpy(data, entry->value, value_size);
            }
            data += value_size;
        } else {
            // No entry defined -> NULL
            memset(data, 0, hm->buf.chunk_size);
            data += hm->buf.chunk_size;
        }

        ++bit_index;
        if (bit_index > 63) {
            bit_index = 0;
            ++free_index;
        }
    }

    // dump free array
    memcpy(data, hm->buf.free, sizeof(uint64) * CEIL_DIV(hm->buf.count, 64));

    return sizeof(hm->buf.count) // hash map count = buffer count
        + hm->buf.count * sizeof(uint64) // table content
        + hm->buf.size; // hash map content + free array
}

// WARNING: Requires hashmap_create first
inline
int64 hashmap_load(HashMap* hm, const byte* data)
{
    uint64 count = SWAP_ENDIAN_LITTLE(*((uint64 *) data));
    data += sizeof(uint64);

    // Load the table content
    for (int i = 0; i < count; ++i) {
        uint64 offset =  SWAP_ENDIAN_LITTLE(*((uint64 *) data));
        data += sizeof(offset);

        // the first element has no offset!
        hm->table[i] = offset || i == 0 ? hm->buf.memory + offset : NULL;
    }

    // This loop here is why it is important to already have an initialized hashmap
    // @question Do we maybe want to change this and not require an initalized hashmap?
    memcpy(hm->buf.memory, data, hm->buf.size);
    data += hm->buf.chunk_size * hm->buf.count;

    // @question don't we have to possibly endian swap check the free array as well?
    memcpy(hm->buf.free, data, sizeof(uint64) * CEIL_DIV(hm->buf.count, 64));

    int64 value_size = hm->buf.chunk_size - sizeof(uint64) - sizeof(char) * MAX_KEY_LENGTH - sizeof(uint64);

    // Switch endian AND turn offsets to pointers
    int32 free_index = 0;
    int32 bit_index = 0;
    for (int32 i = 0; i < hm->buf.count; ++i) {
        if ((hm->buf.free[free_index] & (1ULL << bit_index)) > 0) {
            HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, i);

            // element id
            entry->element_id = SWAP_ENDIAN_LITTLE(entry->element_id);

            // key is already loaded with the memcpy
            // @question Do we even want to use memcpy? We are re-checking all the values here anyways

            // next pointer
            if (entry->next) {
                entry->next = (HashEntry *) (hm->buf.memory + SWAP_ENDIAN_LITTLE((uint64) entry->next));
            }

            if (value_size == 4) {
                ((HashEntryInt32 *) entry)->value = SWAP_ENDIAN_LITTLE(((HashEntryInt32 *) entry)->value);
            } else if (value_size == 8) {
                ((HashEntryInt64 *) entry)->value = SWAP_ENDIAN_LITTLE(((HashEntryInt64 *) entry)->value);
            }
        }
    }

    // How many bytes was read from data
    return sizeof(hm->buf.count) // hash map count = buffer count
        + hm->buf.count * sizeof(uint64) // table content
        + hm->buf.size;
}

#endif