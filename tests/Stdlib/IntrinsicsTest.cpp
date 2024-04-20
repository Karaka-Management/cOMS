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
#include <math.h>

#include "../../Utils/TestUtils.h"
#include "../../Stdlib/Intrinsics.h"

int main(int argc, char** argv)
{
    printf("Intrinsics:\n\n");

    ASSERT_EQUALS_WITH_DELTA(Stdlib::Intrinsics::sqrt(1.234f), sqrt(1.234f), 0.01, "%f", "%f");
    ASSERT_EQUALS_WITH_DELTA(Stdlib::Intrinsics::round(1.234f), round(1.234f), 0.01, "%f", "%f");

    printf("\n\n");

    return 0;
}