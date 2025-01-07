/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_STDLIB_PERFECT_HASH_MAP_H
#define TOS_STDLIB_PERFECT_HASH_MAP_H

#include "Types.h"
#include "HashMap.h"
#include "../hash/GeneralHash.h"
#include "../memory/RingMemory.h"

#define PERFECT_HASH_MAP_MAX_KEY_LENGTH 32
typedef uint64 (*perfect_hash_function)(const char* key, int32 seed);

const perfect_hash_function PERFECT_HASH_FUNCTIONS[] = {
    hash_djb2_seeded,
    hash_sdbm_seeded,
    hash_lose_lose_seeded,
    hash_polynomial_rolling_seeded,
    hash_fnv1a_seeded,
    hash_oat_seeded,
    hash_ejb_seeded
};

struct PerfectHashEntryInt32 {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    int32 value;
};

struct PerfectHashEntryInt64 {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    int64 value;
};

struct PerfectHashEntryUIntPtr {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    uintptr_t value;
};

struct PerfectHashEntryVoidP {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    void* value;
};

struct PerfectHashEntryFloat {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    f32 value;
};

struct PerfectHashEntryStr {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    char value[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
};

struct PerfectHashEntry {
    int64 element_id;
    char key[PERFECT_HASH_MAP_MAX_KEY_LENGTH];
    byte* value;
};

struct PerfectHashMap {
    int32 hash_seed;
    perfect_hash_function hash_function;
    int32 entry_size;

    int32 map_size;
    byte* hash_entries;
};

bool set_perfect_hashmap(PerfectHashMap* hm, const char** keys, int32 key_count, perfect_hash_function hash_func, int32 seed_tries, RingMemory* ring)
{
    int32* indices = (int32 *) ring_get_memory(ring, hm->map_size * sizeof(int32), 4);
    bool is_unique = false;

    int32 seed;
    int32 c = 0;

    while (!is_unique && c < seed_tries) {
        is_unique = true;
        seed = rand();
        memset(indices, 0, hm->map_size * sizeof(int32));

        for (int32 j = 0; j < key_count; ++j) {
            int32 index = hash_func(keys[j], seed) % hm->map_size;
            if (indices[index]) {
                is_unique = false;
                break;
            } else {
                indices[index] = 1;
            }
        }

        ++c;
    }

    if (is_unique) {
        hm->hash_seed = seed;
        hm->hash_function = hash_func;
    }

    return is_unique;
}

bool perfect_hashmap_find_perfect_hash(PerfectHashMap* hm, const char** keys, int32 key_count, int32 seed_trys, RingMemory* ring)
{
    int32* indices = (int32 *) ring_get_memory(ring, hm->map_size * sizeof(int32), 4);
    bool is_unique = false;

    for (int32 i = 0; i < ARRAY_COUNT(PERFECT_HASH_FUNCTIONS); ++i) {
        int32 seed;
        int32 c = 0;

        while (!is_unique && c < seed_trys) {
            is_unique = true;
            seed = rand();
            memset(indices, 0, hm->map_size * sizeof(int32));

            for (int32 j = 0; j < key_count; ++j) {
                int32 index = (PERFECT_HASH_FUNCTIONS[i])(keys[j], seed) % hm->map_size;
                if (indices[index]) {
                    is_unique = false;
                    break;
                } else {
                    indices[index] = 1;
                }
            }

            ++c;
        }

        if (is_unique) {
            hm->hash_seed = seed;
            hm->hash_function = PERFECT_HASH_FUNCTIONS[i];
        }
    }

    return is_unique;
}

// WARNING: element_size = element size + remaining HashEntry data size
void perfect_hashmap_create(PerfectHashMap* hm, int32 count, int32 element_size, RingMemory* ring)
{
    hm->map_size = count;
    hm->entry_size = element_size;
    hm->hash_entries = ring_get_memory(
        ring,
        count * element_size,
        0, true
    );
}

// WARNING: element_size = element size + remaining HashEntry data size
void perfect_hashmap_create(PerfectHashMap* hm, int32 count, int32 element_size, BufferMemory* buf)
{
    hm->map_size = count;
    hm->entry_size = element_size;
    hm->hash_entries = buffer_get_memory(
        buf,
        count * element_size,
        0, true
    );
}

// WARNING: element_size = element size + remaining HashEntry data size
void perfect_hashmap_create(PerfectHashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hm->map_size = count;
    hm->entry_size = element_size;
    hm->hash_entries = buf;
}

// Calculates how large a hashmap will be
inline
int64 perfect_hashmap_size(int count, int32 element_size)
{
    return count * element_size;
}

inline
int64 perfect_hashmap_size(const PerfectHashMap* hm)
{
    return hm->entry_size * hm->map_size;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, int32 value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryInt32* entry = (PerfectHashEntryInt32 *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, int64 value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryInt64* entry = (PerfectHashEntryInt64 *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, uintptr_t value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryUIntPtr* entry = (PerfectHashEntryUIntPtr *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, void* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryVoidP* entry = (PerfectHashEntryVoidP *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, f32 value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryFloat* entry = (PerfectHashEntryFloat *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, const char* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryStr* entry = (PerfectHashEntryStr *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    memcpy(entry->value, value, PERFECT_HASH_MAP_MAX_KEY_LENGTH);
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, byte* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryStr* entry = (PerfectHashEntryStr *) (hm->hash_entries + hm->entry_size * index);
    entry->element_id = index;
    str_copy_short(entry->key, key);
    memcpy(entry->value, value, hm->entry_size - sizeof(PerfectHashEntry));
}

inline
PerfectHashEntry* perfect_hashmap_get_entry(const PerfectHashMap* hm, const char* key) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntry* entry = (PerfectHashEntry *) (hm->hash_entries + hm->entry_size * index);

    return *entry->key == '\0' ? NULL : entry;
}

inline
void perfect_hashmap_delete_entry(PerfectHashMap* hm, const char* key) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntry* entry = (PerfectHashEntry *) (hm->hash_entries + hm->entry_size * index);

    // This depends on where we check if an element exists (if we change perfect_hashmap_get_entry this also needs changing)
    *entry->key = '\0';
}

inline
int64 perfect_hashmap_dump(const PerfectHashMap* hm, byte* data)
{
    byte* start = data;

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(hm->map_size);
    data += sizeof(hm->map_size);

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(hm->hash_seed);
    data += sizeof(hm->hash_seed);

    for (int32 i = 0; i < ARRAY_COUNT(PERFECT_HASH_FUNCTIONS); ++i) {
        if (hm->hash_function == PERFECT_HASH_FUNCTIONS[i]) {
            *((int32 *) data) = SWAP_ENDIAN_LITTLE((uint64) i);
            data += sizeof(i);

            break;
        }
    }

    *((int32 *) data) = SWAP_ENDIAN_LITTLE(hm->entry_size);
    data += sizeof(hm->entry_size);

    memcpy(data, hm->hash_entries, hm->map_size * hm->entry_size);
    data += hm->map_size * hm->entry_size;

    return (int64) (data - start);
}

// WARNING: Requires perfect_hashmap_create first
inline
int64 perfect_hashmap_load(PerfectHashMap* hm, const byte* data)
{
    const byte* start = data;

    hm->map_size = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(hm->map_size);

    hm->hash_seed = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(hm->hash_seed);

    hm->hash_function = PERFECT_HASH_FUNCTIONS[*((int32 *) data)];
    data += sizeof(int32);

    hm->entry_size = SWAP_ENDIAN_LITTLE(*((int32 *) data));
    data += sizeof(hm->entry_size);

    memcpy(hm->hash_entries, data, hm->map_size * hm->entry_size);
    data += hm->map_size * hm->entry_size;

    return (int64) (data - start);
}

// WARNiNG: Requires the phm to be initialized already incl. element count and element size etc.
inline
bool perfect_hashmap_from_hashmap(PerfectHashMap* phm, const HashMap* hm, int32 seed_trys, RingMemory* ring)
{
    char** keys = (char **) ring_get_memory(ring, sizeof(char *) * hm->buf.count, 8);

    // Find all keys
    int32 key_index = 0;
    for (int32 i = 0; i < hm->buf.count; ++i) {
        HashEntry* entry = (HashEntry *) hm->table[i];
        while (entry != NULL) {
            keys[key_index++] = entry->key;
            entry = (HashEntry *) entry->next;
        }
    }

    // Check if we can turn it into a perfect hash map
    bool is_perfect = perfect_hashmap_find_perfect_hash(phm, (char **) keys, key_index, seed_trys, ring);
    if (!is_perfect) {
        return false;
    }

    // Fill perfect hash map
    for (int32 i = 0; i < hm->buf.count; ++i) {
        HashEntry* entry = (HashEntry *) hm->table[i];
        while (entry != NULL) {
            perfect_hashmap_insert(phm, entry->key, entry->value);
            entry = (HashEntry *) entry->next;
        }
    }

    return true;
}

#endif