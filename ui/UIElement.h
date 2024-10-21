#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "../stdlib/Types.h"
#include "UIElementType.h"
#include "../object/Vertex.h"

struct UIElementDimension {
	int16 x1;
	int16 y1;
	int16 x2;
	int16 y2;
};

#define UI_ELEMENT_STATE_VISIBLE 1
#define UI_ELEMENT_STATE_ACTIVE 2
#define UI_ELEMENT_STATE_FOCUSED 4
#define UI_ELEMENT_STATE_CLICKED 8
#define UI_ELEMENT_STATE_ANIMATION 16

struct UIElement {
    const char* name;
    int32 id;
    UIElementType type;

    int16 window_id;
    int16 panel_id;

    UIElementDimension dimension;

    int32 state_flag;

    f32 anim_elapsed;

    int16 scroll_x;
    int16 scroll_y;

    // @todo animation state

    // @todo cache vertex result for default, hover etc.
    int32 vertex_count;
    Vertex3DTextureColorIndex* vertices; // WARNING: This is not the official holder of the memory, its in UILayout

    // @todo We could even have a pointer that points into the complete ui array making it possible to simply replace this section
    //      This is something we wanted to do anyway when updating sub regions on the gpu memory
};

#endif