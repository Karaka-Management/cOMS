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
#include "../log/DebugMemory.h"

// The major asset types should have their own asset component system
// All other entities are grouped together in one asset component system
// @question Asset component systems could be created per region -> easy to simulate a specific region
// @bug This means players might not be able to transition from one area to another?!

// @performance There is a huge performance flaw. We CANNOT have an asset only in vram because it always also allocates the ram (asset_data_memory)
struct AssetManagementSystem {
    // @question is this even necessary or could we integrate this directly into the system here?
    HashMap hash_map;

    uint64 ram_size;
    uint64 vram_size;
    uint64 asset_count;
    int32 overhead;
    bool has_changed;

    // The indices of asset_memory and asset_data_memory are always linked

    // General asset memory
    // Fixed chunk size of sizeof(Asset)
    ChunkMemory asset_memory;

    // Actual asset data
    // Chunk size defined during initialization
    ChunkMemory asset_data_memory;

    // @performance Do we really need the linked list, the ChunkMemory should allow us to do some smart stuff
    Asset* first;
    Asset* last;

    // @question do we want to create an extra threaded version? Or a combined one, like we have right now.
    // @question Do we want to add a mutex to assets. This way we don't have to lock the entire ams.
    pthread_mutex_t mutex;
};

void ams_create(AssetManagementSystem* ams, BufferMemory* buf, int32 chunk_size, int32 count, int32 overhead = 0)
{
    // setup hash_map
    hashmap_create(&ams->hash_map, count, sizeof(HashEntryInt64), buf);

    ams->overhead = overhead;

    // setup asset_memory
    chunk_init(&ams->asset_memory, buf, count, sizeof(Asset), 64);

    // setup asset_data_memory
    chunk_init(&ams->asset_data_memory, buf, count, chunk_size, 64);

    ams->first = NULL;
    ams->last = NULL;

    pthread_mutex_init(&ams->mutex, NULL);
}

// WARNING: buf size see ams_get_buffer_size
void ams_create(AssetManagementSystem* ams, byte* buf, int32 chunk_size, int32 count, int32 overhead = 0)
{
    ASSERT_SIMPLE(chunk_size);

    // setup hash_map
    hashmap_create(&ams->hash_map, count, sizeof(HashEntryInt64), buf);

    ams->overhead = overhead;

    // setup asset_memory
    ams->asset_memory.count = count;
    ams->asset_memory.chunk_size = sizeof(Asset);
    ams->asset_memory.last_pos = -1;
    ams->asset_memory.alignment = 64;
    ams->asset_memory.memory = buf;
    ams->asset_memory.free = (uint64 *) (ams->asset_memory.memory + ams->asset_memory.chunk_size * count);

    // setup asset_data_memory
    ams->asset_data_memory.count = count;
    ams->asset_data_memory.chunk_size = chunk_size;
    ams->asset_data_memory.last_pos = -1;
    ams->asset_data_memory.alignment = 64;
    ams->asset_data_memory.memory = (byte *) (ams->asset_memory.free + CEIL_DIV(count, 64));
    ams->asset_data_memory.free = (uint64 *) (ams->asset_data_memory.memory + ams->asset_data_memory.chunk_size * count);

    ams->first = NULL;
    ams->last = NULL;

    pthread_mutex_init(&ams->mutex, NULL);
}

void ams_free(AssetManagementSystem* ams)
{
    pthread_mutex_destroy(&ams->mutex);
}

inline
int32 ams_calculate_chunks(const AssetManagementSystem* ams, int32 byte_size)
{
    return (int32) CEIL_DIV(byte_size + ams->overhead, ams->asset_data_memory.chunk_size);
}

inline
int64 ams_get_buffer_size(int32 count, int32 chunk_size)
{
    return hashmap_size(count, sizeof(HashEntryInt64)) // hash map
        + sizeof(Asset) * count + CEIL_DIV(count, 64) * sizeof(uint64) // asset_memory
        + chunk_size * count + CEIL_DIV(count, 64) * sizeof(uint64); // asset_data_memory
}

inline
void ams_update_stats(AssetManagementSystem* ams)
{
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

    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? sizeof(Asset) : 0
    );

    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? ((Asset *) entry->value)->self + ((Asset *) entry->value)->ram_size : 0
    );

    return entry ? (Asset *) entry->value : NULL;
}

inline
Asset* ams_get_asset(AssetManagementSystem* ams, const char* key, uint64 hash)
{
    HashEntry* entry = hashmap_get_entry(&ams->hash_map, key, hash);

    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? sizeof(Asset) : 0
    );

    DEBUG_MEMORY_READ(
        (uint64) (entry ? (Asset *) entry->value : 0),
        entry ? ((Asset *) entry->value)->self + ((Asset *) entry->value)->ram_size : 0
    );

    return entry ? (Asset *) entry->value : NULL;
}

// @performance We could probably avoid locking by adding a atomic flag to indicate if the value is valid
Asset* thrd_ams_get_asset(AssetManagementSystem* ams, uint64 element) {
    pthread_mutex_lock(&ams->mutex);
    Asset* asset = ams_get_asset(ams, element);
    pthread_mutex_unlock(&ams->mutex);

    return asset;
}

Asset* thrd_ams_get_asset(AssetManagementSystem* ams, const char* key) {
    pthread_mutex_lock(&ams->mutex);
    Asset* asset = ams_get_asset(ams, key);
    pthread_mutex_unlock(&ams->mutex);

    return asset;
}

Asset* thrd_ams_get_asset(AssetManagementSystem* ams, const char* key, uint64 hash) {
    pthread_mutex_lock(&ams->mutex);
    Asset* asset = ams_get_asset(ams, key, hash);
    pthread_mutex_unlock(&ams->mutex);

    return asset;
}

// @todo implement defragment command to optimize memory layout since the memory layout will become fragmented over time

// @performance This function is VERY important, check if we can optimize it
// We could probably optimize the threaded version by adding a atomic_set_release(asset->is_loaded, true);
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
    asset->size = elements; // Crucial for freeing
    asset->ram_size = (ams->asset_memory.chunk_size + ams->asset_data_memory.chunk_size) * elements;

    DEBUG_MEMORY_RESERVE((uint64) asset->self, elements * ams->asset_data_memory.chunk_size, 180);

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

void ams_garbage_collect(AssetManagementSystem* ams, uint64 time, uint64 dt)
{
    Asset* asset = ams->first;

    while (asset) {
        // @performance We cannot just remove ram and keep vram. This is a huge flaw
        if (asset->can_garbage_collect_ram && asset->can_garbage_collect_vram && time - asset->last_access <= dt) {
            ams_free_asset(ams, asset);
        }

        asset = asset->next;
    }
}

void ams_garbage_collect(AssetManagementSystem* ams)
{
    Asset* asset = ams->first;

    while (asset) {
        // @performance We cannot just remove ram and keep vram. This is a huge flaw
        if (asset->can_garbage_collect_ram && asset->can_garbage_collect_vram) {
            ams_free_asset(ams, asset);
        }

        asset = asset->next;
    }
}

void thrd_ams_garbage_collect(AssetManagementSystem* ams, uint64 time, uint64 dt)
{
    pthread_mutex_lock(&ams->mutex);
    ams_garbage_collect(ams, time, dt);
    pthread_mutex_unlock(&ams->mutex);
}

void thrd_ams_garbage_collect(AssetManagementSystem* ams)
{
    pthread_mutex_lock(&ams->mutex);
    ams_garbage_collect(ams);
    pthread_mutex_unlock(&ams->mutex);
}

Asset* thrd_ams_reserve_asset(AssetManagementSystem* ams, const char* name, uint32 elements = 1) {
    pthread_mutex_lock(&ams->mutex);
    Asset* asset = ams_reserve_asset(ams, name, elements);
    pthread_mutex_unlock(&ams->mutex);

    return asset;
}

Asset* thrd_ams_reserve_asset_start(AssetManagementSystem* ams, const char* name, uint32 elements = 1) {
    pthread_mutex_lock(&ams->mutex);

    return ams_reserve_asset(ams, name, elements);
}

void thrd_ams_reserve_asset_end(AssetManagementSystem* ams) {
    pthread_mutex_unlock(&ams->mutex);
}

#endif