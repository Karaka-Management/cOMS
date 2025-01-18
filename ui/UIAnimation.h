#ifndef TOS_UI_ANIMATION_H
#define TOS_UI_ANIMATION_H

#include "../stdlib/Types.h"
#include "../animation/AnimationEaseType.h"

#define UI_ANIMATION_ACTIVE_FLAG 0x80000000

struct UIAnimationState {
    uint64 start;
    uint32 duration;
    // Element specific use (e.g. cursor uses 0/1 for visible/invisible for blinking)
    // The highest bit indicates if the animation is active or not
    int32 state;
    AnimationEaseType anim_type;
};

struct UIAnimation {
    uint32 duration;
    AnimationEaseType anim_type;
};

#endif