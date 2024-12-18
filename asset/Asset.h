/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ASSET_H
#define TOS_ASSET_H

#include "../stdlib/Types.h"
#include "AssetType.h"

#define MAX_ASSET_NAME_LENGTH 32

struct Asset {
    // The id is the same as its location in memory/in the ams array
    // This is is only an internal id and NOT the same as a db id (e.g. player id)
    uint64 internal_id;

    // Could be 0 if there is no official id
    uint64 official_id;

    // @performance This is bad, this uses the same name as the hashmap
    // We effectively store the asset name twice which shouldn't be the case
    char name[MAX_ASSET_NAME_LENGTH];

    AssetType type;

    // Counts the references to this asset
    // e.g. textures
    int32 reference_count;

    // Describes how much ram/vram the asset uses
    // E.g. vram_size = 0 but ram_size > 0 means that it never uses any gpu memory
    uint32 ram_size;
    uint32 vram_size;
    uint64 last_access;

    // Usually 1 but in some cases an ams may hold entities of variable chunk length
    // For textures for example a 128x128 is of size 1 but 256x256 is of size 4
    uint32 size;

    // Variable used for thread safety
    bool is_loaded;

    // Describes if the memory is currently available in ram/vram
    // E.g. an asset might be uploaded to the gpu and no longer held in ram (or the other way around)
    bool is_ram;
    bool is_vram;

    // Describes if the asset can be removed/garbage collected IF necessary
    // This however only happens if space is needed
    bool can_garbage_collect_ram;
    bool can_garbage_collect_vram;

    Asset* next;
    Asset* prev;

    // An asset can reference up to N other entities
    // This allows us to quickly update the other entities
    // Example: A player pulls N mobs
    // @bug This means there are hard limits on how many mobs can be pulled by a player
    Asset* references[50];
    uint64 free_references; // bits show which is free

    // Actual memory address and specific asset data
    byte* self;
};

#endif