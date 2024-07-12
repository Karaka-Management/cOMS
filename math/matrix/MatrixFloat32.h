/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_MATH_MATRIX_FLOAT32_H
#define TOS_MATH_MATRIX_FLOAT32_H

#include "../../stdlib/Intrinsics.h"
#include "../../utils/MathUtils.h"

void mat3_identity_f32(float* matrix)
{
    matrix[0] = 1.0f; matrix[1] = 0.0f; matrix[2] = 0.0f;
    matrix[3] = 0.0f; matrix[4] = 1.0f; matrix[5] = 0.0f;
    matrix[6] = 0.0f; matrix[7] = 0.0f; matrix[8] = 1.0f;
}

void mat3_identity_f32(__m128* matrix)
{
    matrix[0] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
    matrix[1] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
    matrix[2] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
}

void mat4_identity_f32(float* matrix)
{
    matrix[0] = 1.0f;  matrix[1] = 0.0f;  matrix[2] = 0.0f;  matrix[3] = 0.0f;
    matrix[4] = 0.0f;  matrix[5] = 1.0f;  matrix[6] = 0.0f;  matrix[7] = 0.0f;
    matrix[8] = 0.0f;  matrix[9] = 0.0f;  matrix[10] = 1.0f; matrix[11] = 0.0f;
    matrix[12] = 0.0f; matrix[13] = 0.0f; matrix[14] = 0.0f; matrix[15] = 1.0f;
}

void mat4_identity_f32(__m128* matrix)
{
    matrix[0] = _mm_set_ps(1.0f, 0.0f, 0.0f, 0.0f);
    matrix[1] = _mm_set_ps(0.0f, 1.0f, 0.0f, 0.0f);
    matrix[2] = _mm_set_ps(0.0f, 0.0f, 1.0f, 0.0f);
    matrix[3] = _mm_set_ps(0.0f, 0.0f, 0.0f, 1.0f);
}

void mat_translate_f32(float* matrix, float dx, float dy, float dz)
{
    matrix[0] = 1;   matrix[1] = 0;   matrix[2] = 0;   matrix[3] = 0;
    matrix[4] = 0;   matrix[5] = 1;   matrix[6] = 0;   matrix[7] = 0;
    matrix[8] = 0;   matrix[9] = 0;   matrix[10] = 1;  matrix[11] = 0;
    matrix[12] = dx; matrix[13] = dy; matrix[14] = dz; matrix[15] = 1;
}

// x, y, z need to be normalized
void mat3_rotate(float* matrix, float x, float y, float z, float angle)
{
    float s = sinf_approx(angle);
    float c = cosf_approx(angle);
    float m = 1 - c;

    float mx = m * x;
    float my = m * y;
    float mz = m * z;

    float zs = z * s;
    float xs = x * s;
    float ys = y * s;

    float mxy = mx * y;
    float mzx = mz * x;
    float myz = my * z;

    matrix[0] = mx * x + c;
    matrix[1] = mxy - zs;
    matrix[2] = mzx + ys;
    matrix[3] = 0;

    matrix[4] = mxy + zs;
    matrix[5] = my * y + c;
    matrix[6] = myz - xs;
    matrix[7] = 0;

    matrix[8] = mzx - ys;
    matrix[9] = myz + xs;
    matrix[10] = mz * z + c;
    matrix[11] = 0;

    matrix[12] = 0;
    matrix[13] = 0;
    matrix[14] = 0;
    matrix[15] = 1;
}

void mat3vec3_mult(const float* matrix, const float* vector, float* result)
{
    result[0] = matrix[0] * vector[0] + matrix[1] * vector[1] + matrix[2] * vector[2];
    result[1] = matrix[3] * vector[0] + matrix[4] * vector[1] + matrix[5] * vector[2];
    result[2] = matrix[6] * vector[0] + matrix[7] * vector[1] + matrix[8] * vector[2];

    /*
    for (int i = 0; i < 3; ++i) {
        result[i] = matrix[i * 3 + 0] * vector[0]
            + matrix[i * 3 + 1] * vector[1]
            + matrix[i * 3 + 2] * vector[2];
    }
    */
}

void mat3vec3_mult_sse(const float* matrix, const float* vector, float* result)
{
    __m128 vec = _mm_loadu_ps(vector);
    vec = _mm_insert_ps(vec, _mm_setzero_ps(), 0x30); // vec[3] = 0

    for (int i = 0; i < 3; ++i) {
        __m128 row = _mm_loadu_ps(&matrix[i * 3]);
        row = _mm_insert_ps(row, _mm_setzero_ps(), 0x30);  // row[3] = 0

        __m128 dot = _mm_dp_ps(row, vec, 0xF1);

        result[i] = _mm_cvtss_f32(dot);
    }
}

void mat3vec3_mult_sse(const __m128* matrix, const __m128* vector, float* result)
{
    for (int i = 0; i < 3; ++i) {
        __m128 dot = _mm_dp_ps(matrix[i], *vector, 0xF1);

        result[i] = _mm_cvtss_f32(dot);
    }
}

void mat3vec3_mult_sse(const __m128* matrix, const __m128* vector, __m128* result)
{
    for (int i = 0; i < 4; ++i) {
        result[i] = _mm_dp_ps(matrix[i], *vector, 0xF1);
    }
}

void mat4vec4_mult(const float* matrix, const float* vector, float* result)
{
    result[0] = matrix[0] * vector[0] + matrix[1] * vector[1] + matrix[2] * vector[2] + matrix[3] * vector[3];
    result[1] = matrix[4] * vector[0] + matrix[5] * vector[1] + matrix[6] * vector[2] + matrix[7] * vector[3];
    result[2] = matrix[8] * vector[0] + matrix[9] * vector[1] + matrix[10] * vector[2] + matrix[11] * vector[3];
    result[3] = matrix[12] * vector[0] + matrix[13] * vector[1] + matrix[14] * vector[2] + matrix[15] * vector[3];

    /*
    for (int i = 0; i < 4; ++i) {
        result[i] = matrix[i * 4 + 0] * vector[0]
            + matrix[i * 4 + 1] * vector[1]
            + matrix[i * 4 + 2] * vector[2]
            + matrix[i * 4 + 3] * vector[3];
    }
    */
}

void mat4vec4_mult_sse(const float* matrix, const float* vector, float* result)
{
    __m128 vec = _mm_loadu_ps(vector);

    for (int i = 0; i < 4; ++i) {
        __m128 row = _mm_loadu_ps(&matrix[i * 4]);
        __m128 dot = _mm_dp_ps(row, vec, 0xF1);

        result[i] = _mm_cvtss_f32(dot);
    }
}

void mat4vec4_mult_sse(const __m128* matrix, const __m128* vector, float* result)
{
    for (int i = 0; i < 4; ++i) {
        __m128 dot = _mm_dp_ps(matrix[i], *vector, 0xF1);

        result[i] = _mm_cvtss_f32(dot);
    }
}

void mat4vec4_mult_sse(const __m128* matrix, const __m128* vector, __m128* result)
{
    for (int i = 0; i < 4; ++i) {
        result[i] = _mm_dp_ps(matrix[i], *vector, 0xF1);
    }
}

// @question Consider to replace with 1d array
void frustum_planes(float planes[6][4], int radius, float *matrix) {
    // @todo make this a setting
    float znear = 0.125;
    float zfar = radius * 32 + 64;

    float *m = matrix;

    planes[0][0] = m[3] + m[0];
    planes[0][1] = m[7] + m[4];
    planes[0][2] = m[11] + m[8];
    planes[0][3] = m[15] + m[12];

    planes[1][0] = m[3] - m[0];
    planes[1][1] = m[7] - m[4];
    planes[1][2] = m[11] - m[8];
    planes[1][3] = m[15] - m[12];

    planes[2][0] = m[3] + m[1];
    planes[2][1] = m[7] + m[5];
    planes[2][2] = m[11] + m[9];
    planes[2][3] = m[15] + m[13];

    planes[3][0] = m[3] - m[1];
    planes[3][1] = m[7] - m[5];
    planes[3][2] = m[11] - m[9];
    planes[3][3] = m[15] - m[13];

    planes[4][0] = znear * m[3] + m[2];
    planes[4][1] = znear * m[7] + m[6];
    planes[4][2] = znear * m[11] + m[10];
    planes[4][3] = znear * m[15] + m[14];

    planes[5][0] = zfar * m[3] - m[2];
    planes[5][1] = zfar * m[7] - m[6];
    planes[5][2] = zfar * m[11] - m[10];
    planes[5][3] = zfar * m[15] - m[14];
}

void mat_frustum(
    float *matrix, float left, float right, float bottom,
    float top, float znear, float zfar)
{
    float temp, temp2, temp3, temp4;
    temp = 2.0 * znear;
    temp2 = right - left;
    temp3 = top - bottom;
    temp4 = zfar - znear;

    matrix[0] = temp / temp2;
    matrix[1] = 0.0;
    matrix[2] = 0.0;
    matrix[3] = 0.0;

    matrix[4] = 0.0;
    matrix[5] = temp / temp3;
    matrix[6] = 0.0;
    matrix[7] = 0.0;

    matrix[8] = (right + left) / temp2;
    matrix[9] = (top + bottom) / temp3;
    matrix[10] = (-zfar - znear) / temp4;
    matrix[11] = -1.0;

    matrix[12] = 0.0;
    matrix[13] = 0.0;
    matrix[14] = (-temp * zfar) / temp4;
    matrix[15] = 0.0;
}

void mat_perspective(
    float *matrix, float fov, float aspect,
    float znear, float zfar)
{
    float ymax, xmax;
    ymax = znear * tanf_approx(fov * OMS_PI / 360.0);
    xmax = ymax * aspect;

    mat_frustum(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

void mat_ortho(
    float *matrix,
    float left, float right, float bottom, float top, float near, float far)
{
    float rl_delta = right - left;
    float tb_delta = top - bottom;
    float fn_delta = far - near;

    matrix[0] = 2 / rl_delta;
    matrix[1] = 0;
    matrix[2] = 0;
    matrix[3] = 0;

    matrix[4] = 0;
    matrix[5] = 2 / tb_delta;
    matrix[6] = 0;
    matrix[7] = 0;

    matrix[8] = 0;
    matrix[9] = 0;
    matrix[10] = -2 / fn_delta;
    matrix[11] = 0;

    matrix[12] = -(right + left) / rl_delta;
    matrix[13] = -(top + bottom) / tb_delta;
    matrix[14] = -(far + near) / fn_delta;
    matrix[15] = 1;
}

#endif