#ifndef TOS_UI_ELEMENT_H
#define TOS_UI_ELEMENT_H

#include "../stdlib/Types.h"
#include "UIStyleType.h"
#include "UIAnimation.h"

enum UIElementType : byte {
    UI_ELEMENT_TYPE_BUTTON,
    UI_ELEMENT_TYPE_SELECT,
    UI_ELEMENT_TYPE_INPUT,
    UI_ELEMENT_TYPE_LABEL,
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

    // Are we currently in an animation?
    // @question Do we even need this? Can't we just use the animation state (start == 0)
    UI_ELEMENT_STATE_ANIMATION = 1 << 4,

    // Flag to indicate that the element changed
    // Just checking style_old and style_new is not enough, since we may have an ongoing animation
    // We also need to be able to check the parent element in case the parent element changed position
    //      -> we also need to change position of all child elements (parent sets this flag for all child elements)
    UI_ELEMENT_STATE_CHANGED = 1 << 5,
};

// All pointer like variables are uint32 offset values
// This allows us to copy the data around temp memory without loss of reference
// For arrays we position the data after the UIElement
struct UIElement {
    // @see UIElementState
    byte state_flag;
    UIElementType type;

    // Used to keep track of the current state (= _old) and the next state or state we are transitioning into
    UIStyleType style_old;
    UIStyleType style_new;

    // Used for grouping ui elements (e.g. const ui elements, fairly static elements, dynamic elements)
    byte category;

    f32 zindex;

    // Some elements need information from their parent element
    // @question Do we even need this? When rendering we always start at the root and go down
    // This means we could easily pass the parent element as a parameter in the function call
    // Maybe we still want this in case we ever want to call individual elements?
    uint32 parent;

    //////////////////////////////////////
    // Sub info
    //////////////////////////////////////

    // Every element can have a fixed state, which is element type specific
    // This state is shared across all style_type (see below)
    uint32 state;

    // Every element can have multiple implementations for different states (e.g. hover, focus, ...)
    // These implementations are also element type specific and can be found in here
    uint32 style_types[UI_STYLE_TYPE_SIZE];

    // A UIElement can have child elements (e.g. a Window has labels, buttons, etc...)
    uint16 children_count;

    // The children array comes directly after UIElement
    // uint32* children;

    //////////////////////////////////////
    // Animations
    //////////////////////////////////////

    // A UIElement can have animations
    // e.g. when switching between styles/states or simply repeating animations like hover animations
    // @question Should this even be here or should this be in the state of the element?
    UIAnimationState animation_state;

    // The animation count is defined in the theme file
    uint16 animation_count;

    // Location where the animation information is stored
    // We cannot put it at the end of this UIElement since the length is only known by the theme
    uint32 animations;

    // The animations array comes directly after the children array
    // This means: UIElement + children_count * sizeof(uint32)
    // uint32* animations;

    //////////////////////////////////////
    // Cache
    //////////////////////////////////////

    // We cache the last UI element rendering for re-use in the next frame
    // @question There might be custom UI elements which need more than 2^16 vertices
    uint16 vertex_count;

    // The max vertex count is defined in the theme file
    uint16 vertex_count_max;

    // Offset into the vertex array (NOT in bytes but in vertices)
    uint32 vertices_active;
};

#endif