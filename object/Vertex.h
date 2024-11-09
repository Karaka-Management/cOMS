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
    v3_f32 position;
    v3_f32 normal;
    v2_f32 tex_coord;
    v4_f32 color;
};

struct Vertex3DTextureColor {
    v3_f32 position;
    v2_f32 tex_coord;
    v4_f32 color;
};

struct Vertex3DTextureColorIndex {
    v3_f32 position;
    v2_f32 tex_coord;
    uint32 color;
};

struct Vertex3DColorIndex {
    v3_f32 position;
    uint32 color;
};

struct Vertex2D {
    v2_f32 position;
    v2_f32 tex_coord;
    v4_f32 color;
};

struct Vertex2DTexture {
    v2_f32 position;
    v2_f32 tex_coord;
};

struct Vertex2DColor {
    v2_f32 position;
    v4_f32 color;
};

struct Vertex2DColorIndex {
    v2_f32 position;
    // @bug opengl shaders don't support individual bytes,
    // otherwise we would use byte here for 256 color palettes.
    // Which is bad since the purpose of this was to save 3 bytes by using a color palette
    f32 color;
};

struct VertexRef {
    uint32 data_id;

    uint32 position_id;
    uint32 normal_id;

    uint32 tex_coord_id;
    uint32 sampler_id; // e.g. GL_TEXTURE0

    uint32 color_id;

    uint32 index_id;
};

// Data for the text shader
struct TextShader {
    uint32 program_id;

    uint32 matrix_id;
    uint32 matrix_addr;

    uint32 uv_addr;
    uint32 color_addr;
    uint32 sampler_addr;
};

enum VertexType {
    VERTEX_TYPE_POSITION = 1,
    VERTEX_TYPE_NORMAL = 2,
    VERTEX_TYPE_TEXTURE_COORD = 4,
    VERTEX_TYPE_COLOR = 8,
    VERTEX_TYPE_ALL = 15,
};

#endif