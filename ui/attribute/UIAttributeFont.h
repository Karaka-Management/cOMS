#ifndef TOS_UI_ATTRIBUTE_FONT_H
#define TOS_UI_ATTRIBUTE_FONT_H

#include "../../stdlib/Types.h"

enum UIFontDecoration : byte {
    UI_FONT_DECORATION_UNDERLINE = 1 << 0,
    UI_FONT_DECORATION_ITALIC = 1 << 1,
};

struct UIAttributeFont {
    f32 size;
    uint32 color;
    f32 weight;
    UIAttributeShadow shadow_outer;
    byte decoration;

    // @todo family?
};

#endif