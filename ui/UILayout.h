#ifndef TOS_UI_LAYOUT_H
#define TOS_UI_LAYOUT_H

#include "../stdlib/Mathtypes.h"

#include "UIPosition.h"
#include "UILocation.h"

struct UILayout {
    int x;
    int y;

    int width;
    int height;

    UIPosition position;

    UILocation self;
    UILocation children;

    int margin[4];
    int padding[4];

    int border_width[4];
    v3_int32 border_color;

    v3_int32 color_background;
    v3_int32 color_foreground;

    int image_background;
    int image_width;
    int image_height;

    // Horizontal and vertical scaling can have 2 scalable areas
    int image_horizontal_area1[4];
    int image_horizontal_area2[4];

    int image_vertical_area1[4];
    int image_vertical_area2[4];
    bool image_repeatable;

    int shadow_outer_width[4];
    int shadow_outer_color;

    int shadow_inner_width[4];
    int shadow_inner_color;

    int curser_style;
};

#endif