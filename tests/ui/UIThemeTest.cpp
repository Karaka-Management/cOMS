#include "../TestFramework.h"
#include "../../ui/UITheme.h"
#include "../../system/Allocator.h"

static void test_theme_from_file_txt() {
    RingMemory ui_theme_ring;
    ring_alloc(&ui_theme_ring, 10 * MEGABYTE, 64);

    UIThemeStyle theme;
    theme.data = (byte *) platform_alloc(2 * MEGABYTE);

    theme_from_file_txt(&theme, "./../../GameEditor/assets/themes/default/scene1.themetxt", &ui_theme_ring);

    UIAttributeGroup* group = theme_style_group(&theme, "#cmd_window");
    ASSERT_NOT_EQUALS(group, NULL);
    ASSERT_TRUE(group->attribute_count > 0);

    UIAttribute* attr = ui_attribute_from_group(group, UI_ATTRIBUTE_TYPE_POSITION_X);
    ASSERT_NOT_EQUALS(attr, NULL);
    ASSERT_EQUALS(attr->datatype, UI_ATTRIBUTE_DATA_TYPE_F32);
    ASSERT_EQUALS_WITH_DELTA(attr->value_float, 0.0f, 0.001f);

    ring_free(&ui_theme_ring);
}

static void test_theme_to_data() {
}

static void test_theme_from_data() {
}

#ifdef UBER_TEST
    #ifdef main
        #undef main
    #endif
    #define main UIUIThemeTest
#endif

int main() {
    TEST_INIT(10);

    RUN_TEST(test_theme_from_file_txt);
    RUN_TEST(test_theme_to_data);
    RUN_TEST(test_theme_from_data);

    TEST_FINALIZE();

    return 0;
}
