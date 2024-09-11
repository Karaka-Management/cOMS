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

    // The indices of entity_memory and entity_data_memory are always linked

    // General entity memory
    ChunkMemory entity_memory;

    // Actual entity data
    ChunkMemory entity_data_memory;

    Entity* first;
    Entity* last;
};

struct EntitySchemaSystem {
    // @question is this even necessary or could we integrate this directly into the system here?
    HashMap hash_map;

    // The indices of entity_memory and entity_data_memory are always linked

    // General entity memory
    ChunkMemory entity_memory;

    // Actual entity data
    ChunkMemory entity_data_memory;

    EntitySchema* first;
    EntitySchema* last;
};

#endif