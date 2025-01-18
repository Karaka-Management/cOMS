#ifndef TOS_UI_INPUT_H
#define TOS_UI_INPUT_H

#include "../stdlib/Types.h"
#include "../camera/Camera.h"
#include "attribute/UIAttributeBorder.h"
#include "attribute/UIAttributeShadow.h"
#include "attribute/UIAttributeFont.h"
#include "attribute/UIAttributeBackground.h"
#include "attribute/UIAttributeDimension.h"
#include "UIAnimation.h"
#include "UIStyleType.h"
#include "UIElement.h"
#include "../math/Evaluator.h"

enum UIInputType : byte {
    UI_INPUT_TYPE_TEXT,
    UI_INPUT_TYPE_NUMERIC,
    UI_INPUT_TYPE_DATE,
    UI_INPUT_TYPE_DATE_TIME,
    UI_INPUT_TYPE_TIME,
};

struct UIInputState {
    char content[512];
    char* content_ref;
    uint16 cursor_pos_x;
    uint16 cursor_pos_y;
    UIAnimationState animation;
    UIInputType type;
    int32 min_value;
    int32 max_value;
    uint16 max_input_length;
};

struct UIInput {
    UIAttributeDimension dimension;
    byte opacity; // 1 byte alpha channel
    byte padding;

    // Animation used to get into this style
    UIAnimation animation;

    UIAttributeFont font;
    UIAttributeBackground background;

    UIAttributeBorder border;
    UIAttributeShadow shadow_outer;
    UIAttributeShadow shadow_inner;
};

void ui_input_state_populate(const UIAttributeGroup* group, UIInputState* state) {
    for (int32 i = 0; i < group->attribute_size; ++i) {
        switch (group->attributes[i].attribute_id) {
            case UI_ATTRIBUTE_TYPE_TYPE: {
                    state->type = (UIInputType) group->attributes[i].value_int;
                } break;
            case UI_ATTRIBUTE_TYPE_MIN_VALUE: {
                    state->min_value = group->attributes[i].value_int;
                } break;
            case UI_ATTRIBUTE_TYPE_MAX_VALUE: {
                    state->max_value = group->attributes[i].value_int;
                } break;
            case UI_ATTRIBUTE_TYPE_MAX_INPUT_LENGTH: {
                    state->max_input_length = (uint16) group->attributes[i].value_int;
                } break;
        }
    }
}

void ui_input_element_populate(
    const UIAttributeGroup* group,
    UIElement* element,
    UIStyleType style_type,
    EvaluatorVariable* variables
) {
    if (element->parent) {
        // @bug How to ensure that the parent is initialized before the child element
        // Currently the order of the initialization depends on the theme file, NOT the layout file
        // We could fix it by loading the style based on the layout order but this would result in many misses when looking up styles
        //      The reason for these misses are, that often only 1-2 style_types exist per element
        UIInput* parent = (UIInput *) element->parent->details[UI_STYLE_TYPE_DEFAULT];
        variables[2].value = parent->dimension.dimension.x;
        variables[3].value = parent->dimension.dimension.y;
        variables[4].value = parent->dimension.dimension.width;
        variables[5].value = parent->dimension.dimension.height;
    }

    UIInput* e = (UIInput *) element->details[style_type];
    // First set all values, which we can set immediately
    for (int32 i = 0; i < group->attribute_size; ++i) {
        switch (group->attributes[i].attribute_id) {
            case UI_ATTRIBUTE_TYPE_DIMENSION_X:
            case UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH:
            case UI_ATTRIBUTE_TYPE_DIMENSION_Y:
            case UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT: {
                    ui_theme_assign_dimension(&e->dimension, &group->attributes[i], 6, variables);
                } break;
        }
    }
}

#endif