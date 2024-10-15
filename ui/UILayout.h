#ifndef TOS_UI_LAYOUT_H
#define TOS_UI_LAYOUT_H

#include "../stdlib/Types.h"

#include "UIElement.h"
#include "UILocation.h"

struct UILayout {
    int32 ui_deadzone_count = 5;
    UIElementDimension ui_deadzone[5];

    int32 element_hoverable_count;
	UIElementDimension* elements_hoverable;

    int32 element_interactible_count;
	UIElementDimension* elements_interactible;

    int32 element_count;
    UIElement* element;
};

#endif