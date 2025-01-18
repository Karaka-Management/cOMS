#ifndef TOS_UI_ATTRIBUTE_BACKGROUND_STYLE_H
#define TOS_UI_ATTRIBUTE_BACKGROUND_STYLE_H

#include "../../stdlib/Types.h"

enum UIBackgroundStyle : byte {
    UI_BACKGROUND_STYLE_NONE = 1 << 0,
    UI_BACKGROUND_STYLE_COLOR_IMG = 1 << 1, // 0 = color, 1 = img
    UI_BACKGROUND_STYLE_STRETCH = 1 << 2, // 0 = none, 1 = stretch
    UI_BACKGROUND_STYLE_REPEAT = 1 << 3, // 0 = none, 1 = repeat
};

struct UIAttributeBackground {
    UIBackgroundStyle background_style;
    union {
        void* background_image;
        uint32 background_color;
    };
};

#endif