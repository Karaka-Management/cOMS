#ifndef TOS_UI_WINDOW_H
#define TOS_UI_WINDOW_H

#include "../stdlib/Types.h"
#include "attribute/UIAttributeBorder.h"
#include "attribute/UIAttributeShadow.h"
#include "attribute/UIAttributeFont.h"
#include "attribute/UIAttributeBackground.h"
#include "attribute/UIAttributeDimension.h"
#include "UIAnimation.h"
#include "UIStyleType.h"

struct UIWindowState {
};

struct UIWindow {
    UIAttributeDimension dimension;
    UIAnimation animation;
    byte padding;
    byte alignment;
    byte opacity;

    uintptr_t background;
    UIBackgroundStyle background_style;
    UIAttributeBorder border;
    UIAttributeShadow shadow_outer;
    UIAttributeShadow shadow_inner;

    UIWindow* styles[UI_STYLE_TYPE_SIZE];
};

#endif