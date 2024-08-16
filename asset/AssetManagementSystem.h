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
#include "AssetType.h"
#include "../memory/ChunkMemory.h"
#include "../utils/TestUtils.h"

// The major asset types should have their own asset component system
// All other entities are grouped together in one asset component system
// @question Asset component systems could be created per region -> easy to simulate a specific region
// @bug This means players might not be able to transition from one area to another?!

struct AssetManagementSystem {
    // The indices of asset_memory and asset_data_memory are always linked

    // General asset memory
    ChunkMemory asset_memory;

    // Actual asset data
    ChunkMemory asset_data_memory;

    Asset* first;
    Asset* last;
};

int ams_get_vram_usage(AssetManagementSystem* ams)
{
    uint64 size = 0;
    for (int i = 0; i < ams->asset_memory.count; ++i) {
        size += ((Asset *) (ams->asset_memory.memory))[i].vram_size;
    }

    return size;
}

void asset_delete(AssetManagementSystem* ams, Asset* asset)
{
    asset->prev->next = asset->next;
    asset->next->prev = asset->prev;

    for (int i = 0; i < asset->size; ++i) {
        chunk_element_free(&ams->asset_memory, asset->internal_id + i);
        chunk_element_free(&ams->asset_data_memory, asset->internal_id + i);
    }
}

// @todo implement defragment command to optimize memory layout since the memory layout will become fragmented over time

Asset* asset_reserve(AssetManagementSystem* ams, uint64 elements = 1)
{
    int64 free_asset = chunk_reserve(&ams->asset_memory, elements, true);
    ASSERT_SIMPLE(free_asset >= 0);

    chunk_reserve_index(&ams->asset_data_memory, free_asset, elements, true);

    Asset* asset = (Asset *) chunk_get_memory(&ams->asset_memory, free_asset);
    asset->internal_id = free_asset;
    asset->self = chunk_get_memory(&ams->asset_data_memory, free_asset);
    asset->ram_size = ams->asset_memory.chunk_size * elements;

    // @performance Do we really want a double linked list. Are we really using this feature or is the free_index enough?
    if (free_asset > 0 && free_asset < ams->asset_memory.count - 1) {
        Asset* next = ams->first;
        while (next->next->internal_id < asset->internal_id && next->internal_id < ams->asset_memory.count) {
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

    return asset;
}

#endif