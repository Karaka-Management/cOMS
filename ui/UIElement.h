#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "UIElementType.h"
#include "UIAlignment.h"
#include "UIAnchor.h"
#include "../stdlib/Types.h"

struct UIElement {
    int id;
    UIElementType type;

    int window_id;
    int panel_id;

    float x;
    float y;
    float width;
    float height;

    UIAlignH align_h;
    UIAlignV align_v;
    UIAnchor anchor;

    bool is_visible;
    bool is_active;
    bool is_focused;

    v4_f32 color;
    v4_f32 background_color;
};

#endif