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
#include "../object/Vertex.h"
#include "../stdlib/HashMap.h"
#include "AssetType.h"

#define MAX_ASSET_NAME_LENGTH 32

struct Asset {
    // The id is the same as its location in memory/in the ams array
    // This is is only an internal id and NOT the same as a db id (e.g. player id)
    uint64 internal_id;

    // Could be 0 if there is no official id
    uint64 official_id;

    char name[MAX_ASSET_NAME_LENGTH];

    AssetType type;

    // Counts the references to this asset
    // e.g. textures
    int reference_count;

    // Describes how much ram/vram the asset uses
    // E.g. vram_size = 0 but ram_size > 0 means that it never uses any gpu memory
    uint64 ram_size;
    uint64 vram_size;

    // Usually 1 but in some cases an ams may hold entities of variable chunk length
    // For textures for example a 128x128 is of size 1 but 256x256 is of size 4
    uint32 size;

    // Describes if the memory is currently available in ram/vram
    // E.g. an asset might be uploaded to the gpu and no longer held in ram (or the other way around)
    bool is_ram;
    bool is_vram;

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