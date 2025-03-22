#define UBER_TEST 1

#include "math/EvaluatorTest.cpp"
#include "memory/ChunkMemoryTest.cpp"
#include "memory/RingMemoryTest.cpp"
#include "stdlib/HashMapTest.cpp"
#include "ui/UILayoutTest.cpp"
#include "ui/UIThemeTest.cpp"
#include "utils/BitUtilsTest.cpp"
#include "utils/EndianUtilsTest.cpp"
#include "utils/StringUtilsTest.cpp"
#include "utils/MathUtilsTest.cpp"
#include "utils/UtilsTest.cpp"

#ifdef UBER_TEST
    #ifdef main
        #undef main
    #endif
#endif

int main() {
    TEST_HEADER();

    MathEvaluatorTest();
    MemoryChunkMemoryTest();
    MemoryRingMemoryTest();
    StdlibHashMapTest();
    UIUILayoutTest();
    UIUIThemeTest();
    UtilsBitUtilsTest();
    UtilsStringUtilsTest();
    UtilsMathUtilsTest();
    UtilsUtilsTest();

    TEST_FOOTER();

    return _test_global_assert_error_count ? 1 : 0;
}