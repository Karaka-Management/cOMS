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
#include "../stdlib/Mathtypes.h"
#include "../math/matrix/MatrixFloat32.h"

void make_character(
    float *data,
    float x, float y, float n, float m, char c)
{
    float *d = data;

    // Texture atlas is 16 characters
    // 1 / 16 = 0.0625
    float a = 0.0625;
    float b = 0.0625 * 2;

    // ascii offset
    int w = c - 32;

    float du = (w % 16) * a;
    float dv = 1 - (w / 16) * b - b;

    // Quad data (2 triangles)
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y - m;
    *(d++) = du + a; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y - m;
    *(d++) = du + 0; *(d++) = dv;
    *(d++) = x + n; *(d++) = y + m;
    *(d++) = du + a; *(d++) = dv + b;
    *(d++) = x - n; *(d++) = y + m;
    *(d++) = du + 0; *(d++) = dv + b;
}

void font_string_dimension(const char *str, v2_int32* dim, const int* width_lookup)
{
    size_t length = strlen(str);
    int width  = 0;

    for (int i = 0; i < length; ++i) {
        if (str[i] == '\n') {
            if (width > dim->x) {
                dim->x = width;
            }

            width = 0;
            ++dim->y;
        }

        width += width_lookup[str[i]];
    }

     if (width > dim->x) {
        dim->x = width;
    }

    if (width > 0) {
        ++dim->y;
    }
}

inline
void entity_world_space(float* world_space, const float* local_space, const float* model_mat)
{
    mat4vec4_mult(model_mat, local_space, world_space);
}

inline
void entity_view_space(float* view_space, const float* world_space, const float* view_mat)
{
    mat4vec4_mult(view_mat, world_space, view_space);
}

inline
void entity_clip_space(float* clip_space, const float* view_space, const float* projection_mat)
{
    mat4vec4_mult(projection_mat, view_space, clip_space);
}

inline
void entity_clip_space_mat(float* result_mat, const float* model_mat, const float* view_mat, const float* projection_mat)
{
    float temp[16];
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
void entity_clip_space_mat_sse(float* result_mat, const float* model_mat, const float* view_mat, const float* projection_mat)
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
void entity_clip_space_from_local(float* clip_space, const float* local_space, const float* mat)
{
    mat4vec4_mult(mat, local_space, clip_space);
}

inline
void entity_clip_space_from_local_sse(float* clip_space, const float* local_space, const float* mat)
{
    mat4vec4_mult_sse(mat, local_space, clip_space);
}

inline
void entity_screen_space(float* screen_space, const float* clip_space, const float* viewport_mat)
{
    // @todo implement
}

inline
void entity_world_space_sse(float* world_space, const float* local_space, const float* model_mat)
{
    mat4vec4_mult_sse(model_mat, local_space, world_space);
}

inline
void entity_view_space_sse(float* view_space, const float* world_space, const float* view_mat)
{
    mat4vec4_mult_sse(view_mat, world_space, view_space);
}

inline
void entity_clip_space_sse(float* clip_space, const float* view_space, const float* projection_mat)
{
    mat4vec4_mult_sse(projection_mat, view_space, clip_space);
}

inline
void entity_screen_space_sse(float* screen_space, const float* clip_space, const float* viewport_mat)
{
    // @todo implement
}

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

inline
void entity_screen_space_sse(__m128* screen_space, const __m128* clip_space, const __m128* viewport_mat)
{
    // @todo implement
}

#endif