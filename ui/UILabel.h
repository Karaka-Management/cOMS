#ifndef TOS_UI_LABEL_H
#define TOS_UI_LABEL_H

#include "../stdlib/Types.h"
#include "attribute/UIAttributeFont.h"
#include "attribute/UIAttributeDimension.h"
#include "UIStyleType.h"
#include "UIElement.h"
#include "UITheme.h"
#include "UILayout.h"
#include "../object/Vertex.h"

struct UILabelState {
    char* content;
};

struct UILabel {
    UIAttributeDimension dimension;
    byte opacity; // 1 byte alpha channel
    UIAttributeFont font;
};

void ui_label_state_serialize(const UILabelState* __restrict state, byte** __restrict pos)
{
    (void *) state;
    (void **) pos;
}

void ui_label_state_unserialize(UILabelState* __restrict state, const byte** __restrict pos)
{
    (void *) state;
    (void **) pos;
}

void ui_label_state_populate(const UIAttributeGroup* __restrict group, UILabelState* __restrict state)
{
    (void *) group;
    (void *) state;
}

void ui_label_element_serialize(const UILabel* __restrict details, byte** __restrict pos)
{
    (void *) details;
    (void **) pos;
}

void ui_label_element_unserialize(UILabel* __restrict details, const byte** __restrict pos)
{
    (void *) details;
    (void **) pos;
}

void ui_label_element_populate(
    UILayout* layout,
    const UIAttributeGroup* __restrict  group,
    UILabel* __restrict label,
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
            case UI_ELEMENT_TYPE_VIEW_WINDOW: {
                    UIWindow* parent_window = (UIWindow *) (layout->data + parent->style_types[UI_STYLE_TYPE_ACTIVE]);
                    parent_dimension = &parent_window->dimension.dimension;
                } break;
            case UI_ELEMENT_TYPE_VIEW_PANEL: {
                    UIPanel* parent_window = (UIPanel *) (layout->data + parent->style_types[UI_STYLE_TYPE_ACTIVE]);
                    parent_dimension = &parent_window->dimension.dimension;
                } break;
            case UI_ELEMENT_TYPE_BUTTON: {
                    UIButton* parent_button = (UIButton *) (layout->data + parent->style_types[UI_STYLE_TYPE_ACTIVE]);
                    parent_dimension = &parent_button->dimension.dimension;
                } break;
            case UI_ELEMENT_TYPE_INPUT: {
                    UIInput* parent_input = (UIInput *) (layout->data + parent->style_types[UI_STYLE_TYPE_ACTIVE]);
                    parent_dimension = &parent_input->dimension.dimension;
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
                    ui_theme_assign_dimension(&label->dimension, &attributes[i], 6, variables);
                } break;
            case UI_ATTRIBUTE_TYPE_FONT_NAME:
            case UI_ATTRIBUTE_TYPE_FONT_COLOR:
            case UI_ATTRIBUTE_TYPE_FONT_SIZE:
            case UI_ATTRIBUTE_TYPE_FONT_WEIGHT:
            case UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT: {
                    ui_theme_assign_font(&label->font, &attributes[i]);
                } break;
        }
    }
}

int32 ui_label_element_update(UILayout* layout, UIElement* element)
{
    UILabel* label = (UILabel *) (layout->data + element->style_types[element->style_new]);
    UILabelState* state = (UILabelState *) (layout->data  + element->state);

    return vertex_text_create(
        layout->vertices_active + element->vertices_active, element->zindex,
        label->dimension.dimension, label->font.alignment,
        layout->font, state->content,
        label->font.size, label->font.color
        //label->font.weight
    ).z;
}

#endif