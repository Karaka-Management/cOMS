#ifndef TOS_UI_LAYOUT_H
#define TOS_UI_LAYOUT_H

#include "../stdlib/Types.h"
#include "../stdlib/HashMap.h"
#include "UIElement.h"

// Modified for every scene
struct UILayout {
    int32 ui_deadzone_size = 5;
    UIElementDimension ui_deadzone[5];

    int32 element_hoverable_size;
    int32 element_hoverable_pos;
	UIElementDimension* elements_hoverable;

    int32 element_interactible_size;
    int32 element_interactible_pos;
	UIElementDimension* elements_interactible;

    // @question Since we use a hashmap below, do we even need the size?
    //      Isn't the size exactly the same as the hash_map buf size
    int32 element_size;
    int32 element_pos;
    HashMap hash_map; // Used to directly find element by name

    // @question Do we even need this or should the hashmap values be the elements directly?
    //  In other places (e.g. theme) we simply define a byte* data variable which actually holds the info.
    UIElement* elements;

    int32 vertex_size;
    int32 vertex_pos;
    Vertex3DTextureColorIndex* vertices;
};

#endif