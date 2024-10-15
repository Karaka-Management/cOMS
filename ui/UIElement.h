#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "UIElementType.h"
#include "UIAlignment.h"
#include "UIAnchor.h"
#include "../stdlib/Types.h"

struct UIElementDimension {
	int16 x1;
	int16 y1;
	int16 x2;
	int16 y2;
};

struct UIElement {
    int id;
    UIElementType type;

    int window_id;
    int panel_id;

    UIElementDimension dimension;

    UIAlignH align_h;
    UIAlignV align_v;
    UIAnchor anchor;

    bool is_visible;
    bool is_active;
    bool is_focused;

    int16 scroll_x;
    int16 scroll_y;

    // @todo animation state
};

#endif