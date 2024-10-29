/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ASSET_MANAGEMENT_SYSTEM_H
#define TOS_ASSET_MANAGEMENT_SYSTEM_H

#include <string.h>
#include "../stdlib/Types.h"
#include "Asset.h"
#include "../memory/ChunkMemory.h"
#include "../utils/TestUtils.h"
#include "../stdlib/HashMap.h"

// The major asset types should have their own asset component system
// All other entities are grouped together in one asset component system
// @question Asset component systems could be created per region -> easy to simulate a specific region
// @bug This means players might not be able to transition from one area to another?!

struct AssetManagementSystem {
    // @question is this even necessary or could we integrate this directly into the system here?
    HashMap hash_map;

    uint64 ram_size;
    uint64 vram_size;
    uint64 asset_count;
    bool has_changed;

    // The indices of asset_memory and asset_data_memory are always linked

    // General asset memory
    ChunkMemory asset_memory;

    // Actual asset data
    ChunkMemory asset_data_memory;

    Asset* first;
    Asset* last;
};

void ams_create(AssetManagementSystem* ams, BufferMemory* buf, int32 chunk_size, int32 count)
{
    // setup hash_map
    hashmap_create(&ams->hash_map, count, sizeof(HashEntryInt64), buf);

    // setup asset_memory
    ams->asset_memory.count = count;
    ams->asset_memory.chunk_size = sizeof(Asset);
    ams->asset_memory.last_pos = -1;
    ams->asset_memory.alignment = 1;
    ams->asset_memory.memory = buffer_get_memory(buf, sizeof(Asset) * count);
    ams->asset_memory.free = (uint64 *) buffer_get_memory(buf, CEIL_DIV(count, 64) * sizeof(uint64));

    // setup asset_data_memory
    ams->asset_data_memory.count = count;
    ams->asset_data_memory.chunk_size = chunk_size;
    ams->asset_data_memory.last_pos = -1;
    ams->asset_data_memory.alignment = 1;
    ams->asset_data_memory.memory = buffer_get_memory(buf, chunk_size * count, 64);
    ams->asset_data_memory.free = (uint64 *) buffer_get_memory(buf, CEIL_DIV(count, 64) * sizeof(uint64));

    ams->first = NULL;
    ams->last = NULL;
}

inline
int32 ams_calculate_chunks(AssetManagementSystem* ams, int32 byte_size)
{
    return (int32) CEIL_DIV(byte_size, ams->asset_data_memory.chunk_size);
}

inline
int64 ams_get_buffer_size(int count, int chunk_size)
{
    return hashmap_size(count, sizeof(HashEntryInt64)) // hash map
        + sizeof(Asset) * count + CEIL_DIV(count, 64) * sizeof(uint64) // asset_memory
        + chunk_size * count + CEIL_DIV(count, 64) * sizeof(uint64); // asset_data_memory
}

// WARNING: buf size see ams_get_buffer_size
void ams_create(AssetManagementSystem* ams, byte* buf, int chunk_size, int count)
{
    // setup hash_map
    hashmap_create(&ams->hash_map, count, sizeof(HashEntryInt64), buf);

    // setup asset_memory
    ams->asset_memory.count = count;
    ams->asset_memory.chunk_size = sizeof(Asset);
    ams->asset_memory.last_pos = -1;
    ams->asset_memory.alignment = 1;
    ams->asset_memory.memory = buf;
    ams->asset_memory.free = (uint64 *) (ams->asset_memory.memory + sizeof(Asset) * count);

    // setup asset_data_memory
    ams->asset_data_memory.count = count;
    ams->asset_data_memory.chunk_size = chunk_size;
    ams->asset_data_memory.last_pos = -1;
    ams->asset_data_memory.alignment = 1;
    ams->asset_data_memory.memory = (byte *) (ams->asset_data_memory.free + CEIL_DIV(count, 64));
    ams->asset_data_memory.free = (uint64 *) (ams->asset_data_memory.memory + chunk_size * count);

    ams->first = NULL;
    ams->last = NULL;
}

inline
void ams_update_stats(AssetManagementSystem* ams)
{
    // @bug We should check the hash map or the memory status, we could still have old values in here

    ams->vram_size = 0;
    ams->ram_size = 0;
    ams->asset_count = 0;

    Asset* temp_asset = ams->first;

    while (temp_asset) {
        ams->vram_size += temp_asset->vram_size;
        ams->ram_size += temp_asset->ram_size;
        ++ams->asset_count;

        temp_asset = temp_asset->next;
    }

    ams->has_changed = false;
}

inline
uint64 ams_get_asset_count(AssetManagementSystem* ams)
{
    if (ams->has_changed) {
        ams_update_stats(ams);
    }

    return ams->asset_count;
}

inline
uint64 ams_get_vram_usage(AssetManagementSystem* ams)
{
    if (ams->has_changed) {
        ams_update_stats(ams);
    }

    return ams->vram_size;
}

inline
uint64 ams_get_ram_usage(AssetManagementSystem* ams)
{
    if (ams->has_changed) {
        ams_update_stats(ams);
    }

    return ams->ram_size;
}

void ams_free_asset(AssetManagementSystem* ams, Asset* asset)
{
    asset->prev->next = asset->next;
    asset->next->prev = asset->prev;

    hashmap_delete_entry(&ams->hash_map, asset->name);

    for (uint32 i = 0; i < asset->size; ++i) {
        chunk_free_element(&ams->asset_memory, asset->internal_id + i);
        chunk_free_element(&ams->asset_data_memory, asset->internal_id + i);
    }

    ams->has_changed = true;
}

inline
Asset* ams_get_asset(AssetManagementSystem* ams, uint64 element)
{
    return (Asset *) chunk_get_element(&ams->asset_memory, element, false);
}

inline
Asset* ams_get_asset(AssetManagementSystem* ams, const char* key)
{
    HashEntry* entry = hashmap_get_entry(&ams->hash_map, key);

    // @bug entry->value seems to be an address outside of any known buffer, how?
    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? ((Asset *) entry->value)->ram_size + sizeof(Asset) : 0
    );

    return entry ? (Asset *) entry->value : NULL;
}

inline
Asset* ams_get_asset(AssetManagementSystem* ams, const char* key, uint64 index)
{
    HashEntry* entry = hashmap_get_entry(&ams->hash_map, key, index);

    // @bug entry->value seems to be an address outside of any known buffer, how?
    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? ((Asset *) entry->value)->ram_size + sizeof(Asset) : 0
    );

    return entry ? (Asset *) entry->value : NULL;
}

// @todo implement defragment command to optimize memory layout since the memory layout will become fragmented over time

Asset* ams_reserve_asset(AssetManagementSystem* ams, const char* name, uint32 elements = 1)
{
    int64 free_asset = chunk_reserve(&ams->asset_memory, elements, true);
    if (free_asset < 0) {
        ASSERT_SIMPLE(free_asset >= 0);
        return NULL;
    }

    size_t name_length = strlen(name);
    ASSERT_SIMPLE(name_length < MAX_ASSET_NAME_LENGTH - 1);

    Asset* asset = (Asset *) chunk_get_element(&ams->asset_memory, free_asset);
    asset->internal_id = free_asset;

    strncpy(asset->name, name, name_length);
    asset->name[name_length] = '\0';

    hashmap_insert(&ams->hash_map, name, (uintptr_t) asset);

    chunk_reserve_index(&ams->asset_data_memory, free_asset, elements, true);
    asset->self = chunk_get_element(&ams->asset_data_memory, free_asset);
    asset->size = elements; // Curcial for freeing
    asset->ram_size = ams->asset_memory.chunk_size * elements;

    // @performance Do we really want a double linked list. Are we really using this feature or is the free_index enough?
    if (free_asset > 0 && free_asset < ams->asset_memory.count - 1) {
        Asset* next = ams->first;
        while (next->next != NULL
            && next->next->internal_id < asset->internal_id
            && next->internal_id < ams->asset_memory.count
        ) {
            next = next->next;
        }

        asset->prev = next;
        asset->next = asset->prev->next;

        if (asset->next) {
            asset->next->prev = asset;
        } else {
            ams->last = asset;
        }

        asset->prev->next = asset;
    } else if (free_asset == 0) {
        asset->next = ams->first;

        if (ams->first) {
            ams->first->prev = asset;
        }

        ams->first = asset;
    } else if (free_asset == ams->asset_memory.count - 1) {
        asset->prev = ams->last;

        // WARNING: no if here because we assume there is no ECS with just a size of 1
        ams->last->next = asset;
        ams->last = asset;
    }

    ams->has_changed = true;

    return asset;
}

#endif