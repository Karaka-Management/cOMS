#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "../stdlib/Types.h"

enum UIElementType : byte {
    UI_ELEMENT_TYPE_BUTTON,
    UI_ELEMENT_TYPE_SELECT,
    UI_ELEMENT_TYPE_INPUT,
    UI_ELEMENT_TYPE_TEXTAREA,
    UI_ELEMENT_TYPE_IMAGE,
    UI_ELEMENT_TYPE_TEXT,
    UI_ELEMENT_TYPE_LINK,
    UI_ELEMENT_TYPE_TABLE,
    UI_ELEMENT_TYPE_VIEW_WINDOW,
    UI_ELEMENT_TYPE_VIEW_PANEL,
    UI_ELEMENT_TYPE_VIEW_TAB,
    UI_ELEMENT_TYPE_CURSOR,
    UI_ELEMENT_TYPE_SIZE,
};

enum UIElementState : byte {
    UI_ELEMENT_STATE_ACTIVE = 1 << 0,
    UI_ELEMENT_STATE_VISIBLE = 1 << 1,
    UI_ELEMENT_STATE_FOCUSED = 1 << 2,
    UI_ELEMENT_STATE_CLICKABLE = 1 << 3,
    UI_ELEMENT_STATE_ANIMATION = 1 << 4,
};

struct UIElement {
    // @see UIElementState
    byte state_flag;
    UIElementType type;

    UIStyleType style_old;
    UIStyleType style_new;
    UIElement* parent;
    void* state;
    void* details[UI_STYLE_TYPE_SIZE];

    uint16 children_count;
    UIElement** children;
};

#endif