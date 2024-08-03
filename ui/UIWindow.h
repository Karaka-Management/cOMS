#ifndef TOS_UI_WINDOW_H
#define TOS_UI_WINDOW_H

#include "UIElement.h"
#include "UILayout.h"

#include "../animation/AnimationEaseType.h"

enum WindowState {
    WINDOW_STATE_ACIVE,
    WINDOW_STATE_INACTIVE,
    WINDOW_STATE_FOCUS,

    WINDOW_STATE_MOVING,
    WINDOW_STATE_OPENING,
    WINDOW_STATE_CLOSING
};

struct UIWindow {
    UIElement element;

    WindowState state_old;
    WindowState state_new;

    bool is_minimizable;
    bool is_maximizable;
    bool is_movable;
    bool is_resizable;

    // window is only movable when holding this area
    int movable_area[4];

    UILayout* layout_default;
    float layout_open_anim_duration;
    float layout_close_anim_duration;
    float layout_min_anim_duration;
};

#endif