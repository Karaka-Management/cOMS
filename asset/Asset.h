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

struct Asset {
    // A id of 0 means the entity is no longer alive
    // The id is the same as its location in memory/in the ecs array
    // This is is only an internal id and NOT the same as a db id (e.g. player id)
    uint32 internal_id;
    uint32 type;

    // Could be 0 if there is no official id
    uint32 official_id;

    uint32 vao; // vertex buffer
    uint32 vbo; // index buffer
    uint32 ebo; // input layout

    // Counts the references to this entity
    // e.g. textures
    int reference_count;

    // Describes how much ram/vram the asset uses
    // E.g. vram_size = 0 but ram_size > 0 means that it never uses any gpu memory
    uint32 ram_size;
    uint32 vram_size;

    // Usually 1 but in some cases an ECS may hold entities of variable chunk length
    // For textures for example a 128x128 is of size 1 but 256x256 is of size 4
    uint32 size;

    // Describes if the memory is currently available in ram/vram
    // E.g. a entity might be uploaded to the gpu and no longer held in ram (or the other way around)
    bool is_ram;
    bool is_vram;

    Asset* next;
    Asset* prev;

    // A entity can reference up to N other entities
    // This allows us to quickly update the other entities
    // Example: A player pulls N mobs
    // @bug This means there are hard limits on how many mobs can be pulled by a player
    Asset* entity_references[50];
    bool free_entity_references[50];

    // Actual memory address
    byte* self;
};

#endif