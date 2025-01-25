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

typedef uint64 (*PerfectHashFunction)(const char* key, int32 seed);

const PerfectHashFunction PERFECT_HASH_FUNCTIONS[] = {
    hash_djb2_seeded,
    hash_sdbm_seeded,
    hash_lose_lose_seeded,
    hash_polynomial_rolling_seeded,
    hash_fnv1a_seeded,
    hash_oat_seeded,
    hash_ejb_seeded
};

struct PerfectHashEntryInt32 {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    int32 value;
};

struct PerfectHashEntryInt64 {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    int64 value;
};

struct PerfectHashEntryUIntPtr {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    uintptr_t value;
};

struct PerfectHashEntryVoidP {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    void* value;
};

struct PerfectHashEntryFloat {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    f32 value;
};

struct PerfectHashEntryStr {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    char value[HASH_MAP_MAX_KEY_LENGTH];
};

struct PerfectHashEntry {
    char key[HASH_MAP_MAX_KEY_LENGTH];
    byte* value;
};

struct PerfectHashMap {
    int32 hash_seed;
    PerfectHashFunction hash_function;
    uint32 entry_size;

    uint32 map_size;
    byte* hash_entries;
};

PerfectHashMap* perfect_hashmap_prepare(PerfectHashMap* hm, const char** keys, int32 key_count, int32 seed_trys, RingMemory* ring)
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

            return hm;
        }
    }

    return NULL;
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

    LOG_LEVEL_2("Created PerfectHashMap for %n elements with %n B per element", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}});
}

// WARNING: element_size = element size + remaining HashEntry data size
void perfect_hashmap_create(PerfectHashMap* hm, int32 count, int32 element_size, byte* buf)
{
    hm->map_size = count;
    hm->entry_size = element_size;
    hm->hash_entries = buf;

    LOG_LEVEL_2("Created PerfectHashMap for %n elements with %n B per element", {{LOG_DATA_INT32, &count}, {LOG_DATA_INT32, &element_size}});
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
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, int64 value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryInt64* entry = (PerfectHashEntryInt64 *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, uintptr_t value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryUIntPtr* entry = (PerfectHashEntryUIntPtr *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, void* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryVoidP* entry = (PerfectHashEntryVoidP *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, f32 value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryFloat* entry = (PerfectHashEntryFloat *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    entry->value = value;
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, const char* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryStr* entry = (PerfectHashEntryStr *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    memcpy(entry->value, value, HASH_MAP_MAX_KEY_LENGTH);
}

inline
void perfect_hashmap_insert(PerfectHashMap* hm, const char* key, byte* value) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntryStr* entry = (PerfectHashEntryStr *) (hm->hash_entries + hm->entry_size * index);
    str_copy_short(entry->key, key);
    memcpy(entry->value, value, hm->entry_size - sizeof(PerfectHashEntry));
}

inline
PerfectHashEntry* perfect_hashmap_get_entry(const PerfectHashMap* hm, const char* key) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntry* entry = (PerfectHashEntry *) (hm->hash_entries + hm->entry_size * index);

    return str_compare(entry->key, key) == 0 ? entry : NULL;
}

inline
void perfect_hashmap_delete_entry(PerfectHashMap* hm, const char* key) {
    int32 index = hm->hash_function(key, hm->hash_seed) % hm->map_size;
    PerfectHashEntry* entry = (PerfectHashEntry *) (hm->hash_entries + hm->entry_size * index);

    if (str_compare(entry->key, key) != 0) {
        return;
    }

    // This depends on where we check if an element exists
    // If we change perfect_hashmap_get_entry this also needs changing
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
    const char** keys = (char **) ring_get_memory(ring, sizeof(char *) * hm->buf.count, 8);

    // Find all keys
    int32 key_index = 0;
    for (int32 i = 0; i < hm->buf.count; ++i) {
        const HashEntry* entry = (HashEntry *) hashmap_get_entry_by_index((HashMap *) hm, i);
        while (entry != NULL) {
            keys[key_index++] = entry->key;
            entry = (HashEntry *) hashmap_get_entry_by_element((HashMap *) hm, entry->next);
        }
    }

    // Check if we can turn it into a perfect hash map
    PerfectHashMap* is_perfect = perfect_hashmap_prepare(phm, keys, key_index, seed_trys, ring);
    if (!is_perfect) {
        return false;
    }

    // Fill perfect hash map
    for (int32 i = 0; i < hm->buf.count; ++i) {
        const HashEntry* entry = (HashEntry *) hashmap_get_entry_by_index((HashMap *) hm, i);
        while (entry != NULL) {
            perfect_hashmap_insert(phm, entry->key, entry->value);
            entry = (HashEntry *) hashmap_get_entry_by_element((HashMap *) hm, entry->next);
        }
    }

    return true;
}

#endif