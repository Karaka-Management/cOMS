#ifndef TOS_UI_ATTRIBUTE_SHADOW_H
#define TOS_UI_ATTRIBUTE_SHADOW_H

#include "../../stdlib/Types.h"

struct UIAttributeShadow {
    f32 angle;
    uint32 color;
    byte fade;
    byte offset;
};

#endif