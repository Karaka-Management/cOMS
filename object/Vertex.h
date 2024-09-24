/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_OBJECT_VERTEX_H
#define TOS_OBJECT_VERTEX_H

#include "../stdlib/Types.h"

struct Vertex3D {
    float position[3];
    float normal[3];
    uint32 tex_coord[2];
    float color[4];
};

struct Vertex2D {
    float position[2];
    uint32 tex_coord[2];
    float color[4];
};

struct VertexRef {
    uint32 position;
    uint32 normal;
    uint32 tex_coord;
    uint32 color;
    uint32 index;
};

// Data for the text shader
struct TextShader {
    uint32 program_id;
    uint32 matrix_id;
    uint32 uv_id;
    uint32 color_id;
    uint32 sampler_id;
};

enum VertexType {
    VERTEX_TYPE_POSITION = 1,
    VERTEX_TYPE_NORMAL = 2,
    VERTEX_TYPE_TEXTURE_COORD = 4,
    VERTEX_TYPE_COLOR = 8,
    VERTEX_TYPE_ALL = 15
};

#endif