#ifndef TOS_UI_BUTTON_H
#define TOS_UI_BUTTON_H

#include "UIElement.h"
#include "UILayout.h"

#include "../animation/AnimationEaseType.h"

enum ButtonState {
    BUTTON_STATE_DEFAULT,
    BUTTON_STATE_HOVER,
    BUTTON_STATE_CLICK
};

struct UIButton {
    UIElement element;

    ButtonState state_old;
    ButtonState state_new;

    // Is pointer becuase we probably want to have a global layout style that we re-use over and over
    UILayout* layout_default;

    UILayout* layout_hover;
    f32 layout_hover_anim_duration;
    AnimationEaseType layout_hover_anim_style;
    int hover_sound;

    UILayout* layout_click;
    f32 layout_click_anim_duration;
    AnimationEaseType layout_click_anim_style;
    int click_sound;
};

#endif