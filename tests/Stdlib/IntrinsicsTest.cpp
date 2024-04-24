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
    ASSERT_EQUALS_WITH_DELTA((int) Stdlib::Intrinsics::round_to_int(1.234f), (int) roundf(1.234f), 0.01, "%d", "%d");

    //ASSERT_EQUALS_WITH_DELTA(Stdlib::Intrinsics::floor(1.234f), 1.0f, 0.01, "%f", "%f");
    //ASSERT_EQUALS_WITH_DELTA(Stdlib::Intrinsics::round(1.234f), roundf(1.234f), 0.01, "%f", "%f");

    ASSERT_NOT_EQUALS(Stdlib::Intrinsics::hash(123456), Stdlib::Intrinsics::hash(654321), "%d", "%d");

    printf("\n\n");

    return 0;
}