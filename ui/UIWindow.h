#ifndef TOS_UI_WINDOW_H
#define TOS_UI_WINDOW_H

#include "../stdlib/Types.h"
#include "attribute/UIAttributeBorder.h"
#include "attribute/UIAttributeShadow.h"
#include "attribute/UIAttributeFont.h"
#include "attribute/UIAttributeBackground.h"
#include "attribute/UIAttributeDimension.h"
#include "UIAnimation.h"
#include "UIPanel.h"
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

void ui_window_state_serialize(const UIWindowState* __restrict state, byte** __restrict pos)
{
    (void *) state;
    (void **) pos;
}

void ui_window_state_unserialize(UIWindowState* __restrict state, const byte** __restrict pos)
{
    (void *) state;
    (void **) pos;
}

void ui_window_state_populate(const UIAttributeGroup* __restrict group, UIWindowState* __restrict state)
{
    (void *) group;
    (void *) state;
}

void ui_window_element_serialize(const UIWindow* __restrict details, byte** __restrict pos)
{
    (void *) details;
    (void **) pos;
}

void ui_window_element_unserialize(UIWindow* __restrict details, const byte** __restrict pos)
{
    (void *) details;
    (void **) pos;
}

void ui_window_element_populate(
    UILayout* layout,
    const UIAttributeGroup* __restrict  group,
    UIWindow* __restrict window,
    UIElement* parent,
    EvaluatorVariable* __restrict variables
) {
    if (parent) {
        // @bug How to ensure that the parent is initialized before the child element
        // Currently the order of the initialization depends on the theme file, NOT the layout file
        // We could fix it by loading the style based on the layout order but this would result in many misses when looking up styles
        //      The reason for these misses are, that often only 1-2 style_types exist per element

        v4_f32* parent_dimension;
        switch (parent->type) {
            case UI_ELEMENT_TYPE_VIEW_PANEL: {
                    UIPanel* parent_window = (UIPanel *) (layout->data + parent->style_types[UI_STYLE_TYPE_ACTIVE]);
                    parent_dimension = &parent_window->dimension.dimension;
                } break;
            default:
                UNREACHABLE();
        }

        variables[2].value = parent_dimension->x;
        variables[3].value = parent_dimension->y;
        variables[4].value = parent_dimension->width;
        variables[5].value = parent_dimension->height;
    }

    UIAttribute* attributes = (UIAttribute *) (group + 1);

    // First set all values, which we can set immediately
    for (uint32 i = 0; i < group->attribute_count; ++i) {
        switch (attributes[i].attribute_id) {
            case UI_ATTRIBUTE_TYPE_POSITION_X:
            case UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH:
            case UI_ATTRIBUTE_TYPE_POSITION_Y:
            case UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT: {
                    ui_theme_assign_dimension(&window->dimension, &attributes[i], 6, variables);
                } break;
        }
    }
}

#endif