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
#include "../stdlib/Simd.h"
#include "../system/Allocator.h"

// If a hash key is longer than the max key length, we use the last N characters of that key
// The key length is currently chosen to result in 32 byte size for the common case: HashEntryInt32
#define HASH_MAP_MAX_KEY_LENGTH 26

/////////////////////////////
// string key
/////////////////////////////
struct HashEntryInt32 {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    int32 value;
};

struct HashEntryInt64 {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    int64 value;
};

struct HashEntryUIntPtr {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    uintptr_t value;
};

struct HashEntryVoidP {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    void* value;
};

struct HashEntryFloat {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    f32 value;
};

struct HashEntryStr {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    char value[HASH_MAP_MAX_KEY_LENGTH];
};

struct HashEntry {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uint16 next;
    byte* value;
};

/////////////////////////////
// int key
/////////////////////////////
struct HashEntryInt32KeyInt32 {
    int32 key;
    uint16 next;
    int32 value;
};

struct HashEntryInt64KeyInt32 {
    int32 key;
    uint16 next;
    int64 value;
};

struct HashEntryUIntPtrKeyInt32 {
    int32 key;
    uint16 next;
    uintptr_t value;
};

struct HashEntryVoidPKeyInt32 {
    int32 key;
    uint16 next;
    void* value;
};

struct HashEntryFloatKeyInt32 {
    int32 key;
    uint16 next;
    f32 value;
};

struct HashEntryStrKeyInt32 {
    int32 key;
    uint16 next;
    char value[HASH_MAP_MAX_KEY_LENGTH];
};

struct HashEntryKeyInt32 {
    int32 key;
    uint16 next;
    byte* value;
};

// HashMaps are limited to 4GB in total size
struct HashMap {
    // Values are 1-indexed/offset since 0 means not used/found
    uint16* table;

    // @todo We might want to align the ChunkMemory memory to 8byte, currently it's either 4 or 8 byte depending on the length
    ChunkMemory buf;
};

void hashmap_alloc(HashMap* hm, int32 count, int32 element_size)
{
    byte* data = (byte *) platform_alloc(
        count * (sizeof(uint16) + element_size)
        + CEIL_DIV(count, 64) * sizeof(hm->buf.free)
    );

    hm->table = (uint16 *) data;
    chunk_init(&hm->buf, data + sizeof(uint16) * count, count, element_size, 8);

    DEBUG_MEMORY_INIT((uintptr_t) hm->buf.memory, hm->buf.size);
    LOG_INCREMENT_BY(DEBUG_COUNTER_MEM_ALLOC, hm->buf.size);
    LOG_LEVEL_2("Allocated HashMap for %n elements with %n B per element", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}});
}

inline
void hashmap_free(HashMap* hm)
{
    DEBUG_MEMORY_DELETE((uintptr_t) hm->buf.memory, hm->buf.size);

    platform_free((void **) &hm->table);

    hm->table = NULL;
    hm->buf.size = 0;
    hm->buf.memory = NULL;
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, RingMemory* ring)
{
    byte* data = ring_get_memory(
        ring,
        count * (sizeof(uint16) + element_size)
        + CEIL_DIV(count, 64) * sizeof(hm->buf.free)
    );

    hm->table = (uint16 *) data;
    chunk_init(&hm->buf, data + sizeof(uint16) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, BufferMemory* buf)
{
    byte* data = buffer_get_memory(
        buf,
        count * (sizeof(uint16) + element_size)
        + CEIL_DIV(count, 64) * sizeof(hm->buf.free)
    );

    hm->table = (uint16 *) data;
    chunk_init(&hm->buf, data + sizeof(uint16) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
}

// WARNING: element_size = element size + remaining HashEntry data size
void hashmap_create(HashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hm->table = (uint16 *) buf;
    chunk_init(&hm->buf, buf + sizeof(uint16) * count, count, element_size, 8);

    LOG_LEVEL_2("Created HashMap for %n elements with %n B per element = %n B", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}, {LOG_DATA_UINT64, &hm->buf.size}});
}

void hashmap_update_data_pointer(HashMap* hm, byte* data)
{
    hm->table = (uint16 *) data;
    hm->buf.memory = data + sizeof(uint16) * hm->buf.count;
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
    return hm->buf.count * sizeof(uint16) + hm->buf.size;
}

/////////////////////////////
// string key
/////////////////////////////
void hashmap_insert(HashMap* hm, const char* key, int32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32* entry = (HashEntryInt32 *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryInt32* tmp = (HashEntryInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, const char* key, int64 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64* entry = (HashEntryInt64 *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryInt64* tmp = (HashEntryInt64 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryInt64 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, const char* key, uintptr_t value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryUIntPtr* entry = (HashEntryUIntPtr *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryUIntPtr* tmp = (HashEntryUIntPtr *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryUIntPtr *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, const char* key, void* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidP* entry = (HashEntryVoidP *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryVoidP* tmp = (HashEntryVoidP *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryVoidP *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, const char* key, f32 value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloat* entry = (HashEntryFloat *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryFloat* tmp = (HashEntryFloat *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryFloat *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, const char* key, const char* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryStr* entry = (HashEntryStr *) chunk_get_element(&hm->buf, element, true);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    str_copy_short(entry->value, value, HASH_MAP_MAX_KEY_LENGTH);
    entry->value[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->next = 0;

    if (hm->table[index]) {
        HashEntryStr* tmp = (HashEntryStr *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryStr *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

HashEntry* hashmap_insert(HashMap* hm, const char* key, byte* value) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, element, true);

    entry->value = (byte *) entry + sizeof(HashEntry);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    memcpy(entry->value, value, hm->buf.chunk_size - sizeof(HashEntry));

    entry->next = 0;

    if (hm->table[index]) {
        HashEntry* tmp = (HashEntry *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntry *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }

    return entry;
}

HashEntry* hashmap_reserve(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, element, true);

    entry->value = (byte *) entry + sizeof(HashEntry);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->next = 0;

    if (hm->table[index]) {
        HashEntry* tmp = (HashEntry *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntry *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }

    return entry;
}

// Returns existing element or element to be filled
HashEntry* hashmap_get_reserve(HashMap* hm, const char* key)
{
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        if (!entry->next) {
            break;
        }

        entry = (HashEntry *) chunk_get_element(&hm->buf, entry->next - 1, false);
    }

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntry* entry_new = (HashEntry *) chunk_get_element(&hm->buf, element, true);

    entry_new->value = (byte *) entry_new + sizeof(HashEntry);

    // Ensure key length
    str_move_to_pos(&key, -HASH_MAP_MAX_KEY_LENGTH);
    str_copy_short(entry_new->key, key, HASH_MAP_MAX_KEY_LENGTH);
    entry_new->key[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    if (entry) {
        entry->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }

    return entry_new;
}

// @performance Some places use this in order to iterate the hashmap that is horrible!!! Use the actual iterate function!
inline
HashEntry* hashmap_get_entry_by_element(HashMap* hm, uint32 element)
{
    return (HashEntry *) chunk_get_element(&hm->buf, element - 1, false);
}

HashEntry* hashmap_get_entry(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        entry = (HashEntry *) chunk_get_element(&hm->buf, entry->next - 1, false);
    }

    return NULL;
}

uint32 hashmap_get_element(const HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    const HashEntry* entry = (const HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, hm->table[index] - 1, false);

    uint32 element_id = hm->table[index];

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return element_id;
        }

        element_id = entry->next;
        entry = (const HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, entry->next - 1, false);
    }

    return 0;
}

inline
uint32 hashmap_get_element_by_entry(const HashMap* hm, const HashEntry* entry)
{
    return chunk_id_from_memory(&hm->buf, (byte *) entry) + 1;
}

// This function only saves one step (omission of the hash function)
// The reason for this is in some cases we can use compile time hashing
HashEntry* hashmap_get_entry(HashMap* hm, const char* key, uint64 hash) {
    hash %= hm->buf.count;
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, hm->table[hash] - 1, false);

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            DEBUG_MEMORY_READ((uintptr_t) entry, sizeof(HashEntry));
            return entry;
        }

        entry = (HashEntry *) chunk_get_element(&hm->buf, entry->next - 1, false);
    }

    return NULL;
}

// @performance If we had a doubly linked list we could delete keys much easier
// However that would make insertion slower
// Maybe we create a nother hashmap that is doubly linked
void hashmap_remove(HashMap* hm, const char* key) {
    uint64 index = hash_djb2(key) % hm->buf.count;
    HashEntry* entry = (HashEntry *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
    HashEntry* prev = NULL;

    uint32 element_id = hm->table[index];

    while (entry != NULL) {
        if (str_compare(entry->key, key, HASH_MAP_MAX_KEY_LENGTH) == 0) {
            if (prev == NULL) {
                hm->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            chunk_free_elements(&hm->buf, element_id - 1);

            return;
        }

        element_id = entry->next;
        prev = entry;
        entry = (HashEntry *) chunk_get_element(&hm->buf, entry->next - 1, false);
    }
}

/////////////////////////////
// int key
/////////////////////////////
void hashmap_insert(HashMap* hm, int32 key, int32 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt32KeyInt32* entry = (HashEntryInt32KeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryInt32KeyInt32* tmp = (HashEntryInt32KeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryInt32KeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, int64 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryInt64KeyInt32* entry = (HashEntryInt64KeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryInt64KeyInt32* tmp = (HashEntryInt64KeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryInt64KeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, uintptr_t value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryUIntPtrKeyInt32* entry = (HashEntryUIntPtrKeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryUIntPtrKeyInt32* tmp = (HashEntryUIntPtrKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryUIntPtrKeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, void* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryVoidPKeyInt32* entry = (HashEntryVoidPKeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryVoidPKeyInt32* tmp = (HashEntryVoidPKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryVoidPKeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, f32 value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryFloatKeyInt32* entry = (HashEntryFloatKeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = value;
    entry->next = 0;

    if (hm->table[index]) {
        HashEntryFloatKeyInt32* tmp = (HashEntryFloatKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryFloatKeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, const char* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryStrKeyInt32* entry = (HashEntryStrKeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;

    str_copy_short(entry->value, value, HASH_MAP_MAX_KEY_LENGTH);
    entry->value[HASH_MAP_MAX_KEY_LENGTH - 1] = '\0';

    entry->next = 0;

    if (hm->table[index]) {
        HashEntryStrKeyInt32* tmp = (HashEntryStrKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryStrKeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

void hashmap_insert(HashMap* hm, int32 key, byte* value) {
    uint64 index = key % hm->buf.count;

    int32 element = chunk_reserve(&hm->buf, 1);
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, element, true);

    entry->key = key;
    entry->value = (byte *) entry + sizeof(HashEntryKeyInt32);

    memcpy(entry->value, value, hm->buf.chunk_size - sizeof(HashEntryKeyInt32));

    entry->next = 0;

    if (hm->table[index]) {
        HashEntryKeyInt32* tmp = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
        while(tmp->next) {
            tmp = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, tmp->next - 1, false);
        }

        tmp->next = (uint16) (element + 1);
    } else {
        hm->table[index] = (uint16) (element + 1);
    }
}

HashEntryKeyInt32* hashmap_get_entry(HashMap* hm, int32 key) {
    uint64 index = key % hm->buf.count;
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);

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
HashEntryKeyInt32* hashmap_get_entry(HashMap* hm, int32 key, uint64 hash) {
    hash %= hm->buf.count;
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, hm->table[hash] - 1, false);

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
    HashEntryKeyInt32* entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, hm->table[index] - 1, false);
    HashEntryKeyInt32* prev = NULL;

    uint32 element_id = hm->table[index];

    while (entry != NULL) {
        if (entry->key == key) {
            if (prev == NULL) {
                hm->table[index] = entry->next;
            } else {
                prev->next = entry->next;
            }

            chunk_free_elements(&hm->buf, element_id - 1);

            return;
        }

        element_id = entry->next;
        prev = entry;
        entry = (HashEntryKeyInt32 *) chunk_get_element(&hm->buf, entry->next - 1, false);
    }
}

inline
int32 hashmap_value_size(const HashMap* hm)
{
    return (uint32) (
        hm->buf.chunk_size
            - sizeof(char) * HASH_MAP_MAX_KEY_LENGTH // key
            - sizeof(uint16) // next element
        );
}

// @question Shouldn't we also store the chunk size etc? Currently not done and expected to be correctly initialized.
inline
int64 hashmap_dump(const HashMap* hm, byte* data, [[maybe_unused]] int32 steps = 8)
{
    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(hm->buf.count);
    data += sizeof(hm->buf.count);

    // Dump the table content
    memcpy(data, hm->table, sizeof(uint16) * hm->buf.count);
    SWAP_ENDIAN_LITTLE_SIMD(
        (uint16 *) data,
        (uint16 *) data,
        sizeof(uint16) * hm->buf.count / 2, // everything is 2 bytes -> super easy to swap
        steps
    );
    data += sizeof(uint16) * hm->buf.count;

    // @bug what if Int32 key?
    int32 value_size = hashmap_value_size(hm);

    // Dumb hash map content = buffer memory
    // Since we are using ChunkMemory we can be smart about it and iterate the chunk memory instead of performing pointer chasing
    int32 free_index = 0;
    int32 bit_index = 0;
    for (uint32 i = 0; i < hm->buf.count; ++i) {
        if (hm->buf.free[free_index] & (1ULL << bit_index)) {
            HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, i);

            // key
            memcpy(data, entry->key, sizeof(entry->key));
            data += sizeof(entry->key);

            // next "pointer"
            *((uint16 *) data) = SWAP_ENDIAN_LITTLE(entry->next);
            data += sizeof(entry->next);

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
        + hm->buf.count * sizeof(uint16) // table content
        + hm->buf.size; // hash map content + free array
}

// WARNING: Requires hashmap_create first
inline
int64 hashmap_load(HashMap* hm, const byte* data, [[maybe_unused]] int32 steps = 8)
{
    uint64 count = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(uint16);

    // Load the table content
    memcpy(hm->table, data, sizeof(uint16) * count);
    SWAP_ENDIAN_LITTLE_SIMD(
        (uint32 *) hm->table,
        (uint32 *) hm->table,
        sizeof(uint16) * count / 4, // everything is 4 bytes -> super easy to swap
        steps
    );
    data += sizeof(uint16) * count;

    // This loop here is why it is important to already have an initialized hashmap
    // @question Do we maybe want to change this and not require an initalized hashmap?
    memcpy(hm->buf.memory, data, hm->buf.size);
    data += hm->buf.chunk_size * hm->buf.count;

    // @question don't we have to possibly endian swap check the free array as well?
    memcpy(hm->buf.free, data, sizeof(uint64) * CEIL_DIV(hm->buf.count, 64));

    // @bug what if Int32 key?
    int32 value_size = hashmap_value_size(hm);

    // Switch endian AND turn offsets to pointers
    uint32 chunk_id = 0;
    chunk_iterate_start(&hm->buf, chunk_id)
        HashEntry* entry = (HashEntry *) chunk_get_element((ChunkMemory *) &hm->buf, chunk_id);

        // key is already loaded with the memcpy
        // @question Do we even want to use memcpy? We are re-checking all the values here anyways

        // next "pointer"
        entry->next = SWAP_ENDIAN_LITTLE(entry->next);

        if (value_size == 4) {
            ((HashEntryInt32 *) entry)->value = SWAP_ENDIAN_LITTLE(((HashEntryInt32 *) entry)->value);
        } else if (value_size == 8) {
            ((HashEntryInt64 *) entry)->value = SWAP_ENDIAN_LITTLE(((HashEntryInt64 *) entry)->value);
        }
    chunk_iterate_end;

    LOG_LEVEL_2("Loaded HashMap: %n B", {{LOG_DATA_UINT64, &hm->buf.size}});

    // How many bytes was read from data
    return sizeof(hm->buf.count) // hash map count = buffer count
        + hm->buf.count * sizeof(uint16) // table content
        + hm->buf.size;
}

#endif