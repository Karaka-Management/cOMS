#include "../TestFramework.h"
#include "../../ui/UILayout.h"
#include "../../ui/UILayout.cpp"
#include "../../system/Allocator.h"

static void test_layout_from_file_txt() {
    RingMemory ui_layout_ring;
    ring_alloc(&ui_layout_ring, 10 * MEGABYTE, 64);

    UILayout layout;
    layout.data = (byte *) platform_alloc(2 * MEGABYTE);

    layout_from_file_txt(&layout, "./../../GameEditor/assets/themes/default/scene1.layouttxt", &ui_layout_ring);

    UIElement* element = layout_get_element(&layout, "cmd_window");
    ASSERT_NOT_EQUALS(element, NULL);

    platform_free((void **) &layout.data);
    ring_free(&ui_layout_ring);
}

static void test_layout_to_data() {
}

static void test_layout_from_data() {
}

static void test_layout_from_theme() {
    RingMemory ui_layout_ring;
    ring_alloc(&ui_layout_ring, 10 * MEGABYTE, 64);

    UILayout layout;
    layout.data = (byte *) platform_alloc(10 * MEGABYTE);
    layout_from_file_txt(&layout, "./../../GameEditor/assets/themes/default/scene1.layouttxt", &ui_layout_ring);

    UIThemeStyle theme1;
    theme1.data = (byte *) platform_alloc(2 * MEGABYTE);
    theme_from_file_txt(&theme1, "./../../GameEditor/assets/themes/default/general.themetxt", &ui_layout_ring);

    UIThemeStyle theme2;
    theme2.data = (byte *) platform_alloc(2 * MEGABYTE);
    theme_from_file_txt(&theme2, "./../../GameEditor/assets/themes/default/scene1.themetxt", &ui_layout_ring);

    Camera camera = {};
    camera.viewport_width = 1024;
    camera.viewport_height = 768;
    layout_from_theme(&layout, &theme1, &camera);
    layout_from_theme(&layout, &theme2, &camera);

    UIElement* element = layout_get_element(&layout, "cmd_window");
    ASSERT_NOT_EQUALS(element, NULL);
    ASSERT_TRUE(element->state > 0);
    ASSERT_TRUE(element->children_count > 0);
    ASSERT_EQUALS(element->type, UI_ELEMENT_TYPE_VIEW_WINDOW);

    platform_free((void **) &layout.data);
    platform_free((void **) &theme1.data);
    platform_free((void **) &theme2.data);
    ring_free(&ui_layout_ring);
}

#ifdef UBER_TEST
    #ifdef main
        #undef main
    #endif
    #define main UIUILayoutTest
#endif

int main() {
    TEST_INIT(10);

    RUN_TEST(test_layout_from_file_txt);
    RUN_TEST(test_layout_to_data);
    RUN_TEST(test_layout_from_data);
    RUN_TEST(test_layout_from_theme);

    TEST_FINALIZE();

    return 0;
}
