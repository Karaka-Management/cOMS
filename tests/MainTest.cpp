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
#include "utils/UtilsTest.cpp"

#ifdef UBER_TEST
    #ifdef main
        #undef main
    #endif
#endif

int main() {
    printf("\nStat Tests   Asserts         Details\n");
    printf("========================================================================================================================\n");

    MathEvaluatorTest();
    MemoryChunkMemoryTest();
    MemoryRingMemoryTest();
    StdlibHashMapTest();
    UIUILayoutTest();
    UIUIThemeTest();
    UtilsBitUtilsTest();
    UtilsStringUtilsTest();
    UtilsUtilsTest();

    printf("========================================================================================================================\n");
    printf(
        "%s %5d   (%5d/%5d)\n\n",
        _test_global_assert_count ? "[NG]" : "[OK]",
        _test_global_count,
        _test_global_assert_count - _test_global_assert_error_count,
        _test_global_assert_count
    );

    return _test_global_assert_error_count ? 1 : 0;
}