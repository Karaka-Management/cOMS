/**
 * Jingga
 *
 * @package   Test
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#include <stdio.h>
#include <stdlib.h>

#include "../../../Utils/TestUtils.h"
#include "../../../Stdlib/SIMD/SIMD_F32.h"
#include "../../../Stdlib/SIMD/SIMD_Helper.h"

float* a_array_4 = (float *) aligned_alloc(32, 4 * sizeof(float));
float* b_array_4 = (float *) aligned_alloc(32, 4 * sizeof(float));
float* expected_array_4 = (float *) aligned_alloc(32, 4 * sizeof(float));
float* result_array_4 = (float *) aligned_alloc(32, 4 * sizeof(float));

float* a_array_8 = (float *) aligned_alloc(32, 8 * sizeof(float));
float* b_array_8 = (float *) aligned_alloc(32, 8 * sizeof(float));
float* expected_array_8 = (float *) aligned_alloc(32, 8 * sizeof(float));
float* result_array_8 = (float *) aligned_alloc(32, 8 * sizeof(float));

float* a_array_16 = (float *) aligned_alloc(32, 16 * sizeof(float));
float* b_array_16 = (float *) aligned_alloc(32, 16 * sizeof(float));
float* expected_array_16 = (float *) aligned_alloc(32, 16 * sizeof(float));
float* result_array_16 = (float *) aligned_alloc(32, 16 * sizeof(float));

int test_operator_plus();
int test_operator_minus();
int test_operator_mul();

int main(int argc, char** argv)
{
    printf("SIMD_F32:\n");

    test_operator_plus();
    test_operator_minus();
    test_operator_mul();

    printf("\n\n");

    return 0;
}

int test_operator_plus()
{
    printf("\noperator+:\n");
    printf("[4]: ");
    if (!Stdlib::SIMD::is_avx_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_4[0] = 0.0f; a_array_4[1] = 1.0f; a_array_4[2] = 2.0f; a_array_4[3] = 3.0f;
    b_array_4[0] = 0.0f; b_array_4[1] = 1.0f; b_array_4[2] = 2.0f; b_array_4[3] = 3.0f;

    expected_array_4[0] = 0.0f; expected_array_4[1] = 2.0f; expected_array_4[2] = 4.0f; expected_array_4[3] = 6.0f;
    Stdlib::SIMD::f32_4_simd expected_simd_4 = Stdlib::SIMD::load_f32_4_simd(expected_array_4);

    Stdlib::SIMD::f32_4_simd a_simd_4 = Stdlib::SIMD::load_f32_4_simd(a_array_4);
    Stdlib::SIMD::f32_4_simd b_simd_4 = Stdlib::SIMD::load_f32_4_simd(b_array_4);

    Stdlib::SIMD::f32_4_simd result_simd_4 = a_simd_4 + b_simd_4;
    Stdlib::SIMD::unload_f32_4_simd(result_simd_4, result_array_4);

    ASSERT_EQUALS_WITH_DELTA(result_array_4[0], expected_array_4[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[1], expected_array_4[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[2], expected_array_4[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[3], expected_array_4[3], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_4 == expected_simd_4));

    printf("\n[8]: ");

    if (!Stdlib::SIMD::is_avx256_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_8[0] = 0.0f; a_array_8[1] = 1.0f; a_array_8[2] = 2.0f; a_array_8[3] = 3.0f;
    a_array_8[4] = 0.0f; a_array_8[5] = 1.0f; a_array_8[6] = 2.0f; a_array_8[7] = 3.0f;

    b_array_8[0] = 0.0f; b_array_8[1] = 1.0f; b_array_8[2] = 2.0f; b_array_8[3] = 3.0f;
    b_array_8[4] = 0.0f; b_array_8[5] = 1.0f; b_array_8[6] = 2.0f; b_array_8[7] = 3.0f;

    expected_array_8[0] = 0.0f; expected_array_8[1] = 2.0f; expected_array_8[2] = 4.0f; expected_array_8[3] = 6.0f;
    expected_array_8[4] = 0.0f; expected_array_8[5] = 2.0f; expected_array_8[6] = 4.0f; expected_array_8[7] = 6.0f;
    Stdlib::SIMD::f32_8_simd expected_simd_8 = Stdlib::SIMD::load_f32_8_simd(expected_array_8);

    Stdlib::SIMD::f32_8_simd a_simd_8 = Stdlib::SIMD::load_f32_8_simd(a_array_8);
    Stdlib::SIMD::f32_8_simd b_simd_8 = Stdlib::SIMD::load_f32_8_simd(b_array_8);

    Stdlib::SIMD::f32_8_simd result_simd_8 = a_simd_8 + b_simd_8;
    Stdlib::SIMD::unload_f32_8_simd(result_simd_8, result_array_8);

    ASSERT_EQUALS_WITH_DELTA(result_array_8[0], expected_array_8[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[1], expected_array_8[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[2], expected_array_8[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[3], expected_array_8[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[4], expected_array_8[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[5], expected_array_8[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[6], expected_array_8[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[7], expected_array_8[7], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_8 == expected_simd_8));

    printf("\n[16]: ");

    if (!Stdlib::SIMD::is_avx512_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_16[0] = 0.0f; a_array_16[1] = 1.0f; a_array_16[2] = 2.0f; a_array_16[3] = 3.0f;
    a_array_16[4] = 0.0f; a_array_16[5] = 1.0f; a_array_16[6] = 2.0f; a_array_16[7] = 3.0f;
    a_array_16[8] = 0.0f; a_array_16[9] = 1.0f; a_array_16[10] = 2.0f; a_array_16[11] = 3.0f;
    a_array_16[12] = 0.0f; a_array_16[13] = 1.0f; a_array_16[14] = 2.0f; a_array_16[15] = 3.0f;

    b_array_16[0] = 0.0f; b_array_16[1] = 1.0f; b_array_16[2] = 2.0f; b_array_16[3] = 3.0f;
    b_array_16[4] = 0.0f; b_array_16[5] = 1.0f; b_array_16[6] = 2.0f; b_array_16[7] = 3.0f;
    b_array_16[8] = 0.0f; b_array_16[9] = 1.0f; b_array_16[10] = 2.0f; b_array_16[11] = 3.0f;
    b_array_16[12] = 0.0f; b_array_16[13] = 1.0f; b_array_16[14] = 2.0f; b_array_16[15] = 3.0f;

    expected_array_16[0] = 0.0f; expected_array_16[1] = 2.0f; expected_array_16[2] = 4.0f; expected_array_16[3] = 6.0f;
    expected_array_16[4] = 0.0f; expected_array_16[5] = 2.0f; expected_array_16[6] = 4.0f; expected_array_16[7] = 6.0f;
    expected_array_16[8] = 0.0f; expected_array_16[9] = 2.0f; expected_array_16[10] = 4.0f; expected_array_16[11] = 6.0f;
    expected_array_16[12] = 0.0f; expected_array_16[13] = 2.0f; expected_array_16[14] = 4.0f; expected_array_16[15] = 6.0f;
    Stdlib::SIMD::f32_16_simd expected_simd_16 = Stdlib::SIMD::load_f32_16_simd(expected_array_16);

    Stdlib::SIMD::f32_16_simd a_simd_16 = Stdlib::SIMD::load_f32_16_simd(a_array_16);
    Stdlib::SIMD::f32_16_simd b_simd_16 = Stdlib::SIMD::load_f32_16_simd(b_array_16);

    Stdlib::SIMD::f32_16_simd result_simd_16 = a_simd_16 + b_simd_16;
    Stdlib::SIMD::unload_f32_16_simd(result_simd_16, result_array_16);

    ASSERT_EQUALS_WITH_DELTA(result_array_16[0], expected_array_16[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[1], expected_array_16[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[2], expected_array_16[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[3], expected_array_16[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[4], expected_array_16[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[5], expected_array_16[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[6], expected_array_16[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[7], expected_array_16[7], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[8], expected_array_16[8], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[9], expected_array_16[9], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[10], expected_array_16[10], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[11], expected_array_16[11], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[12], expected_array_16[12], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[13], expected_array_16[13], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[14], expected_array_16[14], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[15], expected_array_16[15], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_16 == expected_simd_16));

    return 0;
}

int test_operator_minus()
{
    printf("\noperator-:\n");
    printf("[4]: ");
    if (!Stdlib::SIMD::is_avx_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_4[0] = 0.0f; a_array_4[1] = 1.0f; a_array_4[2] = 2.0f; a_array_4[3] = 3.0f;
    b_array_4[0] = 1.0f; b_array_4[1] = 1.0f; b_array_4[2] = 1.0f; b_array_4[3] = 1.0f;

    expected_array_4[0] = -1.0f; expected_array_4[1] = 0.0f; expected_array_4[2] = 1.0f; expected_array_4[3] = 2.0f;
    Stdlib::SIMD::f32_4_simd expected_simd_4 = Stdlib::SIMD::load_f32_4_simd(expected_array_4);

    Stdlib::SIMD::f32_4_simd a_simd_4 = Stdlib::SIMD::load_f32_4_simd(a_array_4);
    Stdlib::SIMD::f32_4_simd b_simd_4 = Stdlib::SIMD::load_f32_4_simd(b_array_4);

    Stdlib::SIMD::f32_4_simd result_simd_4 = a_simd_4 - b_simd_4;
    Stdlib::SIMD::unload_f32_4_simd(result_simd_4, result_array_4);

    ASSERT_EQUALS_WITH_DELTA(result_array_4[0], expected_array_4[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[1], expected_array_4[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[2], expected_array_4[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[3], expected_array_4[3], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_4 == expected_simd_4));

    printf("\n[8]: ");

    if (!Stdlib::SIMD::is_avx256_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_8[0] = 0.0f; a_array_8[1] = 1.0f; a_array_8[2] = 2.0f; a_array_8[3] = 3.0f;
    a_array_8[4] = 0.0f; a_array_8[5] = 1.0f; a_array_8[6] = 2.0f; a_array_8[7] = 3.0f;

    b_array_8[0] = 1.0f; b_array_8[1] = 1.0f; b_array_8[2] = 1.0f; b_array_8[3] = 1.0f;
    b_array_8[4] = 1.0f; b_array_8[5] = 1.0f; b_array_8[6] = 1.0f; b_array_8[7] = 1.0f;

    expected_array_8[0] = -1.0f; expected_array_8[1] = 0.0f; expected_array_8[2] = 1.0f; expected_array_8[3] = 2.0f;
    expected_array_8[4] = -1.0f; expected_array_8[5] = 0.0f; expected_array_8[6] = 1.0f; expected_array_8[7] = 2.0f;
    Stdlib::SIMD::f32_8_simd expected_simd_8 = Stdlib::SIMD::load_f32_8_simd(expected_array_8);

    Stdlib::SIMD::f32_8_simd a_simd_8 = Stdlib::SIMD::load_f32_8_simd(a_array_8);
    Stdlib::SIMD::f32_8_simd b_simd_8 = Stdlib::SIMD::load_f32_8_simd(b_array_8);

    Stdlib::SIMD::f32_8_simd result_simd_8 = a_simd_8 - b_simd_8;
    Stdlib::SIMD::unload_f32_8_simd(result_simd_8, result_array_8);

    ASSERT_EQUALS_WITH_DELTA(result_array_8[0], expected_array_8[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[1], expected_array_8[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[2], expected_array_8[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[3], expected_array_8[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[4], expected_array_8[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[5], expected_array_8[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[6], expected_array_8[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[7], expected_array_8[7], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_8 == expected_simd_8));

    printf("\n[16]: ");

    if (!Stdlib::SIMD::is_avx512_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_16[0] = 0.0f; a_array_16[1] = 1.0f; a_array_16[2] = 2.0f; a_array_16[3] = 3.0f;
    a_array_16[4] = 0.0f; a_array_16[5] = 1.0f; a_array_16[6] = 2.0f; a_array_16[7] = 3.0f;
    a_array_16[8] = 0.0f; a_array_16[9] = 1.0f; a_array_16[10] = 2.0f; a_array_16[11] = 3.0f;
    a_array_16[12] = 0.0f; a_array_16[13] = 1.0f; a_array_16[14] = 2.0f; a_array_16[15] = 3.0f;

    b_array_16[0] = 1.0f; b_array_16[1] = 1.0f; b_array_16[2] = 1.0f; b_array_16[3] = 1.0f;
    b_array_16[4] = 1.0f; b_array_16[5] = 1.0f; b_array_16[6] = 1.0f; b_array_16[7] = 1.0f;
    b_array_16[8] = 1.0f; b_array_16[9] = 1.0f; b_array_16[10] = 1.0f; b_array_16[11] = 1.0f;
    b_array_16[12] = 1.0f; b_array_16[13] = 1.0f; b_array_16[14] = 1.0f; b_array_16[15] = 1.0f;

    expected_array_16[0] = -1.0f; expected_array_16[1] = 0.0f; expected_array_16[2] = 1.0f; expected_array_16[3] = 2.0f;
    expected_array_16[4] = -1.0f; expected_array_16[5] = 0.0f; expected_array_16[6] = 1.0f; expected_array_16[7] = 2.0f;
    expected_array_16[8] = -1.0f; expected_array_16[9] = 0.0f; expected_array_16[10] = 1.0f; expected_array_16[11] = 2.0f;
    expected_array_16[12] = -1.0f; expected_array_16[13] = 0.0f; expected_array_16[14] = 1.0f; expected_array_16[15] = 2.0f;
    Stdlib::SIMD::f32_16_simd expected_simd_16 = Stdlib::SIMD::load_f32_16_simd(expected_array_16);

    Stdlib::SIMD::f32_16_simd a_simd_16 = Stdlib::SIMD::load_f32_16_simd(a_array_16);
    Stdlib::SIMD::f32_16_simd b_simd_16 = Stdlib::SIMD::load_f32_16_simd(b_array_16);

    Stdlib::SIMD::f32_16_simd result_simd_16 = a_simd_16 - b_simd_16;
    Stdlib::SIMD::unload_f32_16_simd(result_simd_16, result_array_16);

    ASSERT_EQUALS_WITH_DELTA(result_array_16[0], expected_array_16[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[1], expected_array_16[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[2], expected_array_16[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[3], expected_array_16[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[4], expected_array_16[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[5], expected_array_16[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[6], expected_array_16[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[7], expected_array_16[7], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[8], expected_array_16[8], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[9], expected_array_16[9], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[10], expected_array_16[10], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[11], expected_array_16[11], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[12], expected_array_16[12], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[13], expected_array_16[13], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[14], expected_array_16[14], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[15], expected_array_16[15], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_16 == expected_simd_16));

    return 0;
}

int test_operator_mul()
{
    printf("\noperator*:\n");
    printf("[4]: ");
    if (!Stdlib::SIMD::is_avx_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_4[0] = 0.0f; a_array_4[1] = 1.0f; a_array_4[2] = 2.0f; a_array_4[3] = 3.0f;
    b_array_4[0] = 0.0f; b_array_4[1] = 1.0f; b_array_4[2] = 2.0f; b_array_4[3] = 3.0f;

    expected_array_4[0] = 0.0f; expected_array_4[1] = 1.0f; expected_array_4[2] = 4.0f; expected_array_4[3] = 9.0f;
    Stdlib::SIMD::f32_4_simd expected_simd_4 = Stdlib::SIMD::load_f32_4_simd(expected_array_4);

    Stdlib::SIMD::f32_4_simd a_simd_4 = Stdlib::SIMD::load_f32_4_simd(a_array_4);
    Stdlib::SIMD::f32_4_simd b_simd_4 = Stdlib::SIMD::load_f32_4_simd(b_array_4);

    Stdlib::SIMD::f32_4_simd result_simd_4 = a_simd_4 * b_simd_4;
    Stdlib::SIMD::unload_f32_4_simd(result_simd_4, result_array_4);

    ASSERT_EQUALS_WITH_DELTA(result_array_4[0], expected_array_4[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[1], expected_array_4[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[2], expected_array_4[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_4[3], expected_array_4[3], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_4 == expected_simd_4));

    printf("\n[8]: ");

    if (!Stdlib::SIMD::is_avx256_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_8[0] = 0.0f; a_array_8[1] = 1.0f; a_array_8[2] = 2.0f; a_array_8[3] = 3.0f;
    a_array_8[4] = 0.0f; a_array_8[5] = 1.0f; a_array_8[6] = 2.0f; a_array_8[7] = 3.0f;

    b_array_8[0] = 0.0f; b_array_8[1] = 1.0f; b_array_8[2] = 2.0f; b_array_8[3] = 3.0f;
    b_array_8[4] = 0.0f; b_array_8[5] = 1.0f; b_array_8[6] = 2.0f; b_array_8[7] = 3.0f;

    expected_array_8[0] = 0.0f; expected_array_8[1] = 1.0f; expected_array_8[2] = 4.0f; expected_array_8[3] = 9.0f;
    expected_array_8[4] = 0.0f; expected_array_8[5] = 1.0f; expected_array_8[6] = 4.0f; expected_array_8[7] = 9.0f;
    Stdlib::SIMD::f32_8_simd expected_simd_8 = Stdlib::SIMD::load_f32_8_simd(expected_array_8);

    Stdlib::SIMD::f32_8_simd a_simd_8 = Stdlib::SIMD::load_f32_8_simd(a_array_8);
    Stdlib::SIMD::f32_8_simd b_simd_8 = Stdlib::SIMD::load_f32_8_simd(b_array_8);

    Stdlib::SIMD::f32_8_simd result_simd_8 = a_simd_8 * b_simd_8;
    Stdlib::SIMD::unload_f32_8_simd(result_simd_8, result_array_8);

    ASSERT_EQUALS_WITH_DELTA(result_array_8[0], expected_array_8[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[1], expected_array_8[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[2], expected_array_8[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[3], expected_array_8[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[4], expected_array_8[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[5], expected_array_8[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[6], expected_array_8[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_8[7], expected_array_8[7], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_8 == expected_simd_8));

    printf("\n[16]: ");

    if (!Stdlib::SIMD::is_avx512_supported()) {
        printf("[\033[33mNot supported\033[0m]");

        return 0;
    }

    a_array_16[0] = 0.0f; a_array_16[1] = 1.0f; a_array_16[2] = 2.0f; a_array_16[3] = 3.0f;
    a_array_16[4] = 0.0f; a_array_16[5] = 1.0f; a_array_16[6] = 2.0f; a_array_16[7] = 3.0f;
    a_array_16[8] = 0.0f; a_array_16[9] = 1.0f; a_array_16[10] = 2.0f; a_array_16[11] = 3.0f;
    a_array_16[12] = 0.0f; a_array_16[13] = 1.0f; a_array_16[14] = 2.0f; a_array_16[15] = 3.0f;

    b_array_16[0] = 0.0f; b_array_16[1] = 1.0f; b_array_16[2] = 2.0f; b_array_16[3] = 3.0f;
    b_array_16[4] = 0.0f; b_array_16[5] = 1.0f; b_array_16[6] = 2.0f; b_array_16[7] = 3.0f;
    b_array_16[8] = 0.0f; b_array_16[9] = 1.0f; b_array_16[10] = 2.0f; b_array_16[11] = 3.0f;
    b_array_16[12] = 0.0f; b_array_16[13] = 1.0f; b_array_16[14] = 2.0f; b_array_16[15] = 3.0f;

    expected_array_16[0] = 0.0f; expected_array_16[1] = 1.0f; expected_array_16[2] = 4.0f; expected_array_16[3] = 9.0f;
    expected_array_16[4] = 0.0f; expected_array_16[5] = 1.0f; expected_array_16[6] = 4.0f; expected_array_16[7] = 9.0f;
    expected_array_16[8] = 0.0f; expected_array_16[9] = 1.0f; expected_array_16[10] = 4.0f; expected_array_16[11] = 9.0f;
    expected_array_16[12] = 0.0f; expected_array_16[13] = 1.0f; expected_array_16[14] = 4.0f; expected_array_16[15] = 9.0f;
    Stdlib::SIMD::f32_16_simd expected_simd_16 = Stdlib::SIMD::load_f32_16_simd(expected_array_16);

    Stdlib::SIMD::f32_16_simd a_simd_16 = Stdlib::SIMD::load_f32_16_simd(a_array_16);
    Stdlib::SIMD::f32_16_simd b_simd_16 = Stdlib::SIMD::load_f32_16_simd(b_array_16);

    Stdlib::SIMD::f32_16_simd result_simd_16 = a_simd_16 * b_simd_16;
    Stdlib::SIMD::unload_f32_16_simd(result_simd_16, result_array_16);

    ASSERT_EQUALS_WITH_DELTA(result_array_16[0], expected_array_16[0], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[1], expected_array_16[1], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[2], expected_array_16[2], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[3], expected_array_16[3], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[4], expected_array_16[4], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[5], expected_array_16[5], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[6], expected_array_16[6], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[7], expected_array_16[7], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[8], expected_array_16[8], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[9], expected_array_16[9], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[10], expected_array_16[10], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[11], expected_array_16[11], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[12], expected_array_16[12], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[13], expected_array_16[13], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[14], expected_array_16[14], 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(result_array_16[15], expected_array_16[15], 0.01, "%f", "%f");
    ASSERT_TRUE(Stdlib::SIMD::all_true(result_simd_16 == expected_simd_16));

    return 0;
}