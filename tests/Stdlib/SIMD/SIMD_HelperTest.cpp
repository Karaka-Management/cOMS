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

#include "../../../Stdlib/SIMD/SIMD_Helper.h"
#include "../../../Utils/TestUtils.h"

int main(int argc, char **argv)
{
    printf("SIMD_Helper:\n");

    if (Stdlib::SIMD::is_avx_supported()) {
        printf("\nAVX is supported");
    } else {
        printf("\033[33m\nAVX is NOT supported\033[0m");
    }

    if (Stdlib::SIMD::is_avx256_supported()) {
        printf("\nAVX 256 is supported");
    } else {
        printf("\033[33m\nAVX 256 is NOT supported\033[0m");
    }

    if (Stdlib::SIMD::is_avx512_supported()) {
        printf("\nAVX 512 is supported");
    } else {
        printf("\033[33m\nAVX 512 is NOT supported\033[0m");
    }

    printf("\n\n");

    return 0;
}
