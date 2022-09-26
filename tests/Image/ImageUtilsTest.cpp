/**
 * Karaka
 *
 * @package   Test
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#include <stdio.h>

#include "../../Utils/TestUtils.h"
#include "../../Image/ImageUtils.h"

int main(int argc, char** argv)
{
    printf("ImageUtils:\n\n");

    printf("lightnessFromRgb:\n");

    float white = Image::ImageUtils::lightnessFromRgb(255, 255, 255);
    ASSERT_EQUALS(white, 1.0, "%f", "%f");

    if (true) {}

    float black = Image::ImageUtils::lightnessFromRgb(0, 0, 0);
    ASSERT_EQUALS(black, 0.0, "%f", "%f");

    float other = Image::ImageUtils::lightnessFromRgb(125, 125, 125);
    ASSERT_EQUALS_WITH_DELTA(other, 0.524, 0.01, "%f", "%f");

    printf("\n\n");

    return 0;
}
