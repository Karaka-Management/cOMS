/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ENTITY_COMPONENT_SYSTEM_H
#define TOS_ENTITY_COMPONENT_SYSTEM_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../memory/ChunkMemory.h"
#include "../utils/TestUtils.h"
#include "../stdlib/HashMap.h"

#include "Entity.h"

struct EntityComponentSystem {
    // @question is this even necessary or could we integrate this directly into the system here?
    HashMap hash_map;

    uint64 ram_size;
    uint64 vram_size;
    uint64 entity_count;
    int32 overhead;

    // @question Do we want this, I would assume this should be almost always true in the final game
    bool has_changed;

    // The indices of entity_memory and entity_data_memory are always linked

    // @question Consider to reset entity_memory->last_pos to 0 before adding a new element
    // This allows us to make the chunk memory more continuous which is better for iteration later on
    // However, adding elements would now be slower. Needs profiling

    // General entity memory
    ChunkMemory entity_memory;

    // Actual entity data
    ChunkMemory entity_data_memory;
};

struct EntitySchemaSystem {
    // @question is this even necessary or could we integrate this directly into the system here?
    HashMap hash_map;

    uint64 ram_size;
    uint64 vram_size;
    uint64 entity_count;
    int32 overhead;
    bool has_changed;

    // The indices of entity_memory and entity_data_memory are always linked

    // General entity memory
    ChunkMemory entity_memory;

    // Actual entity data
    ChunkMemory entity_data_memory;

    EntitySchema* first;
    EntitySchema* last;
};

#endif