#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "UIElementType.h"

struct UIElement {
    int id;
    int type;

    int window_id;
    int panel_id;

    bool is_visible;
    bool is_active;
    bool is_focused;

    UIElementType type;
};

#endif