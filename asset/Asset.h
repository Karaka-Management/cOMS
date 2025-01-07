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

enum AssetState : byte {
    ASSET_STATE_IN_RAM = 1 << 0,
    ASSET_STATE_IN_VRAM = 1 << 1,
    ASSET_STATE_RAM_GC = 1 << 2,
    ASSET_STATE_VRAM_GC = 1 << 3,
};

struct Asset {
    // Could be 0 if there is no official id
    uint32 official_id;

    // @performance We would like to use a bool but windows only supports 32bit atomic values as smallest value
    // Maybe if we would set the IS_LOADED_STATE in the enum as the highest bit we could use the state variable and check it with >=
    int32 is_loaded;

    // Describes how much ram/vram the asset uses
    // E.g. vram_size = 0 but ram_size > 0 means that it never uses any gpu memory
    uint32 ram_size;
    uint32 vram_size;

    uint32 last_access;

    // Usually 1 but in some cases an ams may hold entities of variable chunk length
    // For textures for example a 128x128 is of size 1 but 256x256 is of size 4
    uint16 size;

    // Which asset component is used
    byte component_id;

    byte state;

    // Actual memory address and specific asset data
    byte* self;

    // Counts the references to this asset
    // e.g. textures or entity schemas (NOT entities themselves)
    uint16 reference_count;

    // An asset can reference up to N other assets
    // This allows us to quickly update the other assets
    // Uses official_id
    // @performance This could potentially be bad because many assets will have 0 or only 1-4 references
    uint32 references[12];
};

#endif