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

}

void ui_label_state_unserialize(UILabelState* __restrict state, const byte** __restrict pos)
{

}

void ui_label_state_populate(const UIAttributeGroup* __restrict group, UILabelState* __restrict state)
{

}

void ui_label_element_serialize(const UILabel* __restrict details, byte** __restrict pos)
{

}

void ui_label_element_unserialize(UILabel* __restrict details, const byte** __restrict pos)
{

}

void ui_label_element_populate(
    const UIAttributeGroup* __restrict  group,
    UIElement* __restrict element,
    UIStyleType style_type,
    EvaluatorVariable* __restrict variables
) {
}

int32 ui_label_element_update(UILayout* layout, UIElement* element)
{
    UILabel* label = (UILabel *) (layout->data + element->style_types[element->style_new]);
    UILabelState* state = (UILabelState *) (layout->data  + element->state);

    return vertex_text_create(
        layout->vertices_active + element->vertices_active, element->zindex,
        label->dimension.dimension, label->font.alignment,
        layout->font, state->content,
        label->font.size, label->font.color,
        label->font.weight
    ).z;
}

#endif