/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_HASH_MAP_H
#define TOS_STDLIB_HASH_MAP_H

#include "Types.h"
#include "../hash/GeneralHash.h"
#include "../memory/RingMemory.h"
#include "../memory/BufferMemory.h"
#include "../memory/ChunkMemory.h"
#include "../utils/StringUtils.h"

// WARNING Length of 28 used to ensure perfect padding with element_id and key
#define HASH_MAP_MAX_KEY_LENGTH 28

/////////////////////////////
// string key
/////////////////////////////
struct HashEntryInt32 {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryInt32* next;
    int32 value;
};

struct HashEntryInt64 {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryInt64* next;
    int64 value;
};

struct HashEntryUIntPtr {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryUIntPtr* next;
    uintptr_t value;
};

struct HashEntryVoidP {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryVoidP* next;
    void* value;
};

struct HashEntryFloat {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryFloat* next;
    f32 value;
};

struct HashEntryStr {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntryStr* next;
    char value[HASH_MAP_MAX_KEY_LENGTH];
};

struct HashEntry {
    uint32 element_id;
    char key[HASH_MAP_MAX_KEY_LENGTH];
    HashEntry* next;
    byte* value;
};

/////////////////////////////
// int key
/////////////////////////////
struct HashEntryInt32KeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryInt32KeyInt32* next;
    int32 value;
};

struct HashEntryInt64KeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryInt64KeyInt32* next;
    int64 value;
};

struct HashEntryUIntPtrKeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryUIntPtrKeyInt32* next;
    uintptr_t value;
};

struct HashEntryVoidPKeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryVoidPKeyInt32* next;
    void* value;
};

struct HashEntryFloatKeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryFloatKeyInt32* next;
    f32 value;
};

struct HashEntryStrKeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryStrKeyInt32* next;
    char value[HASH_MAP_MAX_KEY_LENGTH];
};

struct HashEntryKeyInt32 {
    uint32 element_id;
    int32 key;
    HashEntryKeyInt32* next;
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
    chunk_init(&hm->buf, data + sizeof(void *) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
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
    chunk_init(&hm->buf, data + sizeof(void *) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hm->table = (void **) buf;
    chunk_init(&hm->buf, buf + sizeof(void *) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
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

/////////////////////////////
// string key
/////////////////////////////
void hashmap_insert(HashMap* hm, const char* key, int32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32* entry = (HashEntryInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    // @performance Do we really need strncpy? Either use memcpy or strcpy?! Same goes for all the other cases below
    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64* entry = (HashEntryInt64 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryUIntPtr* entry = (HashEntryUIntPtr *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidP* entry = (HashEntryVoidP *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloat* entry = (HashEntryFloat *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryStr* entry = (HashEntryStr *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    strncpy(entry->value, value, HASH_MAP_MAX_KEY_LENGTH);
    entry->value[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

HashEntry* hashmap_insert(HashMap* hm, const char* key, byte* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->value = (byte *) entry + sizeof(HashEntry);

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    return entry;
}

HashEntry* hashmap_reserve(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->value = (byte *) entry + sizeof(HashEntry);

    strncpy(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

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

    return entry;
}

// Returns existing element or element to be filled
HashEntry* hashmap_get_reserve(HashMap* hm, const char* key)
{
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) hm->table[index];

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        if (((HashEntry *) entry->next) == NULL) {
            break;
        }

        entry = (HashEntry *) entry->next;
    }

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry_new = (HashEntry *) chunk_get_element(&hm->buf, element, true);
    entry_new->element_id = element;

    entry_new->value = (byte *) entry_new + sizeof(HashEntry);

    strncpy(entry_new->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry_new->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    if (entry) {
        entry->next = entry_new;
    } else {
        hm->table[index] = entry_new;
    }

    return entry_new;
}

HashEntry* hashmap_get_entry(const HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) hm->table[index];

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        entry = (HashEntry *) entry->next;
    }

    return NULL;
}

// This function only saves one step (omission of the hash function)
// The reason for this is in some cases we can use compile time hashing
HashEntry* hashmap_get_entry(const HashMap* hm, const char* key, uint64 hash) {
    hash %= hm->buf.count;
    HashEntry* entry = (HashEntry *) hm->table[hash];

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        entry = (HashEntry *) entry->next;
    }

    return NULL;
}

// @performance If we had a doubly linked list we could delete keys much easier
// However that would make insertion slower
// Maybe we create a nother hashmap that is doubly linked
void hashmap_remove(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) hm->table[index];
    HashEntry* prev = NULL;

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            if (prev == NULL) {
                hm->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            chunk_free_elements(&hm->buf, entry->element_id);

            return;
        }

        prev = entry;
        entry = entry->next;
    }
}

/////////////////////////////
// int key
/////////////////////////////
void hashmap_insert(HashMap* hm, int32 key, int32 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32KeyInt32* entry = (HashEntryInt32KeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryInt32KeyInt32* tmp = (HashEntryInt32KeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, int64 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64KeyInt32* entry = (HashEntryInt64KeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryInt64KeyInt32* tmp = (HashEntryInt64KeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, uintptr_t value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryUIntPtrKeyInt32* entry = (HashEntryUIntPtrKeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryUIntPtrKeyInt32* tmp = (HashEntryUIntPtrKeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, void* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidPKeyInt32* entry = (HashEntryVoidPKeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryVoidPKeyInt32* tmp = (HashEntryVoidPKeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, f32 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloatKeyInt32* entry = (HashEntryFloatKeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = value;
    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryFloatKeyInt32* tmp = (HashEntryFloatKeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, const char* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryStrKeyInt32* entry = (HashEntryStrKeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;

    strncpy(entry->value, value, HASH_MAP_MAX_KEY_LENGTH);
    entry->value[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryStrKeyInt32* tmp = (HashEntryStrKeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

void hashmap_insert(HashMap* hm, int32 key, byte* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, element, true);
    entry->element_id = element;

    entry->key = key;
    entry->value = (byte *) entry + sizeof(HashEntryKeyInt32);

    memcpy(entry->value, value, hm->buf.chunk_size - sizeof(HashEntryKeyInt32));

    entry->next = NULL;

    if (hm->table[index]) {
        HashEntryKeyInt32* tmp = (HashEntryKeyInt32 *) hm->table[index];
        while(tmp->next) {
            tmp = tmp->next;
        }

        tmp->next = entry;
    } else {
        hm->table[index] = entry;
    }
}

HashEntryKeyInt32* hashmap_get_entry(const HashMap* hm, int32 key) {
    uint64 index = key % hm->buf.count;
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) hm->table[index];

    while (entry != NULL) {
        if (entry->key == key) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntryKeyInt32));
            return entry;
        }

        entry = (HashEntryKeyInt32 *) entry->next;
    }

    return NULL;
}

// This function only saves one step (omission of the hash function)
// The reason for this is in some cases we can use compile time hashing
HashEntryKeyInt32* hashmap_get_entry(const HashMap* hm, int32 key, uint64 hash) {
    hash %= hm->buf.count;
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) hm->table[hash];

    while (entry != NULL) {
        if (entry->key == key) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntryKeyInt32));
            return entry;
        }

        entry = (HashEntryKeyInt32 *) entry->next;
    }

    return NULL;
}

// @performance If we had a doubly linked list we could delete keys much easier
// However that would make insertion slower
// Maybe we create a nother hashmap that is doubly linked
void hashmap_remove(HashMap* hm, int32 key) {
    uint64 index = key % hm->buf.count;
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) hm->table[index];
    HashEntryKeyInt32* prev = NULL;

    while (entry != NULL) {
        if (entry->key == key) {
            if (prev == NULL) {
                hm->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            chunk_free_elements(&hm->buf, entry->element_id);

            return;
        }

        prev = entry;
        entry = entry->next;
    }
}

inline
int32 hashmap_value_size(const HashMap* hm)
{
    return (uint32) (
        hm->buf.chunk_size
            - sizeof(uint32) // element id
            - sizeof(char) * HASH_MAP_MAX_KEY_LENGTH // key
            - sizeof(uintptr_t) // next pointer
        );
}

// @question Shouldn't we also store the hashmap count, chunk size etc? Currently not done and expected to be correctly initialized.
inline
int64 hashmap_dump(const HashMap* hm, byte* data)
{
    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(hm->buf.count);
    data += sizeof(hm->buf.count);

    // Dump the table content where the elements are relative indices/pointers
    for (int32 i = 0; i < hm->buf.count; ++i) {
        *((uint64 *) data) = hm->table[i]
            ? SWAP_ENDIAN_LITTLE((uintptr_t) hm->table[i] - (uintptr_t) hm->buf.memory)
            : 0ULL;
    }
    data += sizeof(uint64) * hm->buf.count;

    // @bug what if Int32 key?
    int32 value_size = hashmap_value_size(hm);

    // Dumb hash map content = buffer memory
    // Since we are using ChunkMemory we can be smart about it and iterate the chunk memory instead of performing pointer chasing
    int32 free_index = 0;
    int32 bit_index = 0;
    for (uint32 i = 0; i < hm->buf.count; ++i) {
        if (hm->buf.free[free_index] & (1ULL << bit_index)) {
            HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, i);

            // element_id
            *((uint32 *) data) = SWAP_ENDIAN_LITTLE(entry->element_id);
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
    uint64 count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(uint32);

    // Load the table content
    for (uint32 i = 0; i < count; ++i) {
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

    // @bug what if Int32 key?
    int32 value_size = hashmap_value_size(hm);

    // Switch endian AND turn offsets to pointers
    int32 chunk_id = 0;
    chunk_iterate_start(&hm->buf, chunk_id)
        HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, chunk_id);

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
    chunk_iterate_end;

    LOG_LEVEL_2("Loaded HashMap: %n B", {{LOG_DATA_UINT64, &hm->buf.size}});

    // How many bytes was read from data
    return sizeof(hm->buf.count) // hash map count = buffer count
        + hm->buf.count * sizeof(uint64) // table content
        + hm->buf.size;
}

#endif