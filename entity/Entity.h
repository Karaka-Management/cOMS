/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_ENTITY_H
#define TOS_ENTITY_H

#include "../stdlib/Types.h"
#include "../stdlib/HashMap.h"
#include "EntityType.h"

#define MAX_ENTITY_NAME_LENGTH 32

struct Entity {
    // The id is the same as its location in memory/in the ecs array
    // This is is only an internal id and NOT the same as a db id (e.g. player id)
    uint64 internal_id;

    EntityType type;

    uint64 last_access;

    // Variable used for thread safety
    bool is_loaded;

    // Describes if the asset can be removed/garbage collected IF necessary
    // This however only happens if space is needed
    bool can_garbage_collect_ram;
    bool can_garbage_collect_vram;

    // Counts the references to this entity
    // e.g. textures
    int16 reference_count;

    // A entity can reference up to N other entities
    // This allows us to quickly update the other entities
    // Example: A player pulls N mobs
    // @bug This means there are hard limits on how many mobs can be pulled by a player
    // @question should this be an entity id?
    Entity* references[50];
    uint64 free_references; // bits show which is free

    // @question should this be an entity id?
    Entity* schema; // This entity represents the schema for this entity (most likely stored in a separate ecs)

    // Actual memory address and specific entity data
    byte* self;
};

struct EntitySchema {
    // The id is the same as its location in memory/in the ecs array
    // This is is only an internal id and NOT the same as a db id (e.g. player id)
    uint64 internal_id;

    // Could be 0 if there is no official id
    uint64 official_id;

    EntityType type;

    // Counts the references to this entity
    // e.g. textures
    int16 reference_count;

    // Describes how much ram/vram the asset uses
    // E.g. vram_size = 0 but ram_size > 0 means that it never uses any gpu memory
    uint64 ram_size;
    uint64 vram_size;
    uint64 last_access;

    // Usually 1 but in some cases an ECS may hold entities of variable chunk length
    // For textures for example a 128x128 is of size 1 but 256x256 is of size 4
    uint32 size;

    // Variable used for thread safety
    bool is_loaded;

    // Describes if the memory is currently available in ram/vram
    // E.g. a entity might be uploaded to the gpu and no longer held in ram (or the other way around)
    bool is_ram;
    bool is_vram;

    // Describes if the asset can be removed/garbage collected IF necessary
    // This however only happens if space is needed
    bool can_garbage_collect_ram;
    bool can_garbage_collect_vram;

    EntitySchema* next;
    EntitySchema* prev;

    // A entity can reference up to N other entities
    // This allows us to quickly update the other entities
    // Example: A player pulls N mobs
    // @bug This means there are hard limits on how many mobs can be pulled by a player
    // @question should this be an entity id?
    EntitySchema* references[50];
    uint64 free_references; // bits show which is free

    // Actual memory address and specific schema data
    byte* self;
};

#endif