/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_RENDER_UTILS_H
#define TOS_GPUAPI_RENDER_UTILS_H

#include <stdio.h>
#include <string.h>
#include "../math/matrix/MatrixFloat32.h"
#include "../font/Font.h"

inline
void vertex_rect_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    uint32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
) {
    if (align_h == UI_ALIGN_H_RIGHT) {
        x -= width;
    } else if (align_h == UI_ALIGN_H_CENTER) {
        x -= width / 2;
    }

    if (align_v == UI_ALIGN_V_TOP) {
        y -= height;
    } else if (align_v == UI_ALIGN_V_CENTER) {
        y -= height / 2;
    }

    // Degenerate triangles
    // They are alternating every loop BUT since we use references they look the same in code
    // WARNING: Before using we must make sure that the 0 index is defined
    //          The easiest way is to just define a "degenerate" starting point
    vertices[*index].position.x = vertices[*index - 1].position.x;
    vertices[*index].position.y = vertices[*index - 1].position.y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = 0;
    vertices[*index].tex_coord.y = 0;
    vertices[*index].color = 0;
    ++(*index);

    vertices[*index].position.x = x;
    vertices[*index].position.y = y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = 0;
    vertices[*index].tex_coord.y = 0;
    vertices[*index].color = 0;
    ++(*index);

    // Rectangle
    vertices[*index].position.x = x;
    vertices[*index].position.y = y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    // Depending on the orientation we either need to add or subtract height -> we use branchless code for that
    vertices[*index].position.x = x;
    vertices[*index].position.y = y + height;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x + width;
    vertices[*index].position.y = y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x + width;
    vertices[*index].position.y = y + height;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);
}

void text_calculate_dimensions(
    f32* __restrict width, f32* __restrict height,
    const Font* __restrict font, const char* text, f32 scale, int32 length
) {
    f32 x = 0;
    f32 y = font->line_height * scale;

    f32 offset_x = 0;

    // @todo remember to restrict to width/height if value > 0 -> force width to remain below certain value

    for (int i = 0; i < length; ++i) {
        int32 character = utf8_get_char_at(text, i);

        if (character == '\n') {
            x = OMS_MAX(x, offset_x);
            y += font->line_height * scale;

            offset_x = 0;

            continue;
        }

        Glyph* glyph = NULL;
        for (int j = 0; j < font->glyph_count; ++j) {
            if (font->glyphs[j].codepoint == character) {
                glyph = &font->glyphs[j];

                break;
            }
        }

        if (!glyph) {
            continue;
        }

        offset_x += (glyph->metrics.width + glyph->metrics.offset_x) * scale;
    }

    *width = OMS_MAX(x, offset_x);
    *height = y;
}

void vertex_text_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    const Font* __restrict font, const char* __restrict text, f32 size, uint32 color_index = 0
) {
    int32 length = utf8_strlen(text);
    float scale = size / font->size;

    // If we do a different alignment we need to pre-calculate the width and height
    if (align_h != 0 || align_v != 0) {
        text_calculate_dimensions(&width, &height, font, text, scale, length);

        if (align_h == UI_ALIGN_H_RIGHT) {
            x -= width;
        } else if (align_h == UI_ALIGN_H_CENTER) {
            x -= width / 2;
        }

        if (align_v == UI_ALIGN_V_TOP) {
            y -= height;
        } else if (align_v == UI_ALIGN_V_CENTER) {
            y -= height / 2;
        }
    }

    f32 offset_x = x;
    for (int i = 0; i < length; ++i) {
        int32 character = utf8_get_char_at(text, i);

        if (character == '\n') {
            y += font->line_height * scale;
            offset_x = x;

            continue;
        }

        Glyph* glyph = NULL;
        for (int j = 0; j < font->glyph_count; ++j) {
            if (font->glyphs[j].codepoint == character) {
                glyph = &font->glyphs[j];

                break;
            }
        }

        if (!glyph) {
            continue;
        }

        vertex_rect_create(
            vertices, index, zindex,
            offset_x, y, glyph->metrics.width * scale, glyph->metrics.height * scale, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
            color_index, glyph->coords.x1, glyph->coords.y1, glyph->coords.x2, glyph->coords.y2
        );

        offset_x += (glyph->metrics.width + glyph->metrics.offset_x) * scale;
    }

    // @question How and where to cut off text out of view (here or somewhere else)
    //      We could just prepare the entire text here but then decide what to render later?
    // @todo If width or height (usually just width) > 0 we use those values for automatic wrapping
    //      This way we can ensure no overflow easily
    // @todo implement line alignment, currently only total alignment is considered
}

inline
void entity_world_space(f32* world_space, const f32* local_space, const f32* model_mat)
{
    mat4vec4_mult(model_mat, local_space, world_space);
}

inline
void entity_view_space(f32* view_space, const f32* world_space, const f32* view_mat)
{
    mat4vec4_mult(view_mat, world_space, view_space);
}

inline
void entity_clip_space(f32* clip_space, const f32* view_space, const f32* projection_mat)
{
    mat4vec4_mult(projection_mat, view_space, clip_space);
}

inline
void entity_clip_space_mat(f32* result_mat, const f32* model_mat, const f32* view_mat, const f32* projection_mat)
{
    f32 temp[16];
    mat4mat4_mult(projection_mat, view_mat, temp);
    mat4mat4_mult(temp, model_mat, result_mat);
}

/**
 * Create the matrix used to transform from local space to clip space
 *
 * This allows us to transform multiple objects with the same matrix
 *
 * Vclip = Mprojection * Mview * Mmodel * Vlocal
 */
void entity_clip_space_mat_sse(f32* result_mat, const f32* model_mat, const f32* view_mat, const f32* projection_mat)
{
    __m128 temp[4];

    __m128 a[4];
    __m128 b[4];

    a[0] = _mm_loadu_ps(projection_mat);
    a[1] = _mm_loadu_ps(&projection_mat[4]);
    a[2] = _mm_loadu_ps(&projection_mat[8]);
    a[3] = _mm_loadu_ps(&projection_mat[12]);

    b[0] = _mm_loadu_ps(view_mat);
    b[1] = _mm_loadu_ps(&view_mat[4]);
    b[2] = _mm_loadu_ps(&view_mat[8]);
    b[3] = _mm_loadu_ps(&view_mat[12]);
    _MM_TRANSPOSE4_PS(b[0], b[1], b[2], b[3]);

    mat4mat4_mult_sse(a, b, temp);

    a[0] = temp[0];
    a[1] = temp[1];
    a[2] = temp[2];
    a[3] = temp[3];

    b[0] = _mm_loadu_ps(model_mat);
    b[1] = _mm_loadu_ps(&model_mat[4]);
    b[2] = _mm_loadu_ps(&model_mat[8]);
    b[3] = _mm_loadu_ps(&model_mat[12]);
    _MM_TRANSPOSE4_PS(b[0], b[1], b[2], b[3]);

    mat4mat4_mult_sse(a, b, temp);
    _mm_store_ps(&result_mat[0], temp[0]);
    _mm_store_ps(&result_mat[4], temp[1]);
    _mm_store_ps(&result_mat[8], temp[2]);
    _mm_store_ps(&result_mat[12], temp[3]);
}

inline
void entity_clip_space_from_local(f32* clip_space, const f32* local_space, const f32* mat)
{
    mat4vec4_mult(mat, local_space, clip_space);
}

inline
void entity_clip_space_from_local_sse(f32* clip_space, const f32* local_space, const f32* mat)
{
    mat4vec4_mult_sse(mat, local_space, clip_space);
}

/*
inline
void entity_screen_space(f32* screen_space, const f32* clip_space, const f32* viewport_mat)
{
    // @todo implement
}
*/

inline
void entity_world_space_sse(f32* world_space, const f32* local_space, const f32* model_mat)
{
    mat4vec4_mult_sse(model_mat, local_space, world_space);
}

inline
void entity_view_space_sse(f32* view_space, const f32* world_space, const f32* view_mat)
{
    mat4vec4_mult_sse(view_mat, world_space, view_space);
}

inline
void entity_clip_space_sse(f32* clip_space, const f32* view_space, const f32* projection_mat)
{
    mat4vec4_mult_sse(projection_mat, view_space, clip_space);
}

/*
inline
void entity_screen_space_sse(f32* screen_space, const f32* clip_space, const f32* viewport_mat)
{
    // @todo implement
}
*/

inline
void entity_world_space_sse(__m128* world_space, const __m128* local_space, const __m128* model_mat)
{
    mat4vec4_mult_sse(model_mat, local_space, world_space);
}

inline
void entity_view_space_sse(__m128* view_space, const __m128* world_space, const __m128* view_mat)
{
    mat4vec4_mult_sse(view_mat, world_space, view_space);
}

inline
void entity_clip_space_sse(__m128* clip_space, const __m128* view_space, const __m128* projection_mat)
{
    mat4vec4_mult_sse(projection_mat, view_space, clip_space);
}

/*
inline
void entity_screen_space_sse(__m128* screen_space, const __m128* clip_space, const __m128* viewport_mat)
{
    // @todo implement
}
*/

#endif