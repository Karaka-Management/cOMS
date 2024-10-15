/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_MESH_H
#define TOS_OBJECT_MESH_H

#include "Vertex.h"
#include "FaceType.h"
#include "../stdlib/Types.h"

#include "../memory/RingMemory.h"

// @todo how to handle different objects and groups?
//      maybe make a mesh hold other meshes?
// @todo handle vertice arrays where for example no texture coordinates are defined/used
struct Mesh {
    byte* data; // memory owner that subdevides into the pointers below

    // @todo Implement the version into the file, currently not implemented
    int32 version;
    uint32 object;

    uint32 group_count;
    uint32* groups;

    // the following section allows us to create 2 types of data
    //      Interleaved: [position normal tex_coord] [color] (elements depend on vertex_type)
    //      Separate: [position] [normal] [tex_coord] [color] (separate array for elements)
    uint32 vertex_type;
    uint32 vertex_count; // can mean only position or combination of position, normal, tex, ...
    uint32 normal_count;
    uint32 tex_coord_count;
    uint32 color_count;
    float* vertices;

    // The following references only exist in some situations
    // depends on the settings above
    float* normals;
    float* tex_coords;
    float* colors;

    uint32 face_type;
    uint32 face_count;
    uint32 face_normal_count;
    uint32 face_tex_coord_count;
    uint32 face_color_count;
    uint32* faces;

    // The following references only exist in some situations
    // depends on the settings above
    uint32* face_textures;
    uint32* face_normals;
    uint32* face_colors;

    // @todo this only works if you have sub meshes e.g. one for body, one for hat, one for weapon etc.
    uint32 vertex_ref;
    uint32 vao;
    uint32 vbo;
    uint32 ebo;

    uint32 material_count;
    uint32* materials;

    uint32 texture;

    uint32 animation_count;
    uint32* animations;

    uint32 hitbox_count;
    uint32* hitboxes;

    uint32 audio_count;
    uint32* audios;

    uint32 mesh_count;
    Mesh* meshes;
};

#endif