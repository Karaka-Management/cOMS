/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_UI_UTILS_H
#define TOS_GPUAPI_UI_UTILS_H

#include <stdio.h>
#include <string.h>

void ui_input_create(Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    uint32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
)
{
    vertex_rect_border_create(
        vertices, index, zindex,
        x, y, width, height, 1, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        12, 0.0f, 0.0f
    );

    vertex_rect_create(
        vertices, index, zindex,
        x + 1, y + 1, width - 2, height - 2, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        14, 0.0f, 0.0f
    );
}


// @todo implement shadow (offset + angle + diffuse) or should this be a shader only thing? if so this would be a problem for us since we are handling text in the same shader as simple shapes
// we might want to implement distance field font atlas
f32 ui_text_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    UIThemeStyle* theme, UIElement* element
) {
    if (element->vertex_count > 0) {
        memcpy(vertices + *index, element->vertices, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);

        return vertices[element->vertex_count - 1].position.x;
    }

    // @performance see comment for setup_theme()

    // Load element data
    HashEntryVoidP* element_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, element->name, element->id);
    UIAttributeGroup* element_group = (UIAttributeGroup *) element_entry->value;

    // Load general style
    UIAttribute* style = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_STYLE);
    HashEntryVoidP* style_entry = NULL;
    UIAttributeGroup* style_group = NULL;

    if (style) {
        style_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, style->value_str);
        style_group = (UIAttributeGroup *) style_entry->value;
    }

    UIAttribute* x;
    UIAttribute* y;

    // Load parent data (for position data)
    UIAttribute* parent = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_PARENT);
    if (parent) {
        HashEntryVoidP* parent_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, parent->value_str);
        UIAttributeGroup* parent_group = (UIAttributeGroup *) parent_entry->value;

        x = ui_attribute_from_group(parent_group, UI_ATTRIBUTE_TYPE_POSITION_X);
        y = ui_attribute_from_group(parent_group, UI_ATTRIBUTE_TYPE_POSITION_Y);

        // @question Do we have more values which can be inherited from the parent?
        //      We don't want to inherit implicit stuff like size, background etc. These things should be defined explicitly
    } else {
        x = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_POSITION_X);
        y = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_POSITION_Y);
    }

    UIAttribute* width = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH);
    UIAttribute* height = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT);
    UIAttribute* align_h = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_ALIGN_H);
    UIAttribute* align_v = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_ALIGN_V);
    UIAttribute* text = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_CONTENT);
    UIAttribute* size = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_FONT_SIZE);
    UIAttribute* color_index = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_FONT_COLOR);

    int32 length = utf8_strlen(text->value_str);
    bool is_ascii = strlen(text->value_str) == length;
    f32 scale = size->value_float / theme->font->size;

    // If we do a different alignment we need to pre-calculate the width and height
    if (align_h != NULL || align_v != NULL) {
        f32 tmp_width = (f32) width->value_int;
        f32 tmp_height = (f32) height->value_int;

        if (align_h != NULL && align_v != NULL) {
            text_calculate_dimensions(&tmp_width, &tmp_height, theme->font, text->value_str, is_ascii, scale, length);
        } else if (align_h != NULL) {
            tmp_width = text_calculate_dimensions_width(theme->font, text->value_str, is_ascii, scale, length);
        } else {
            tmp_height = text_calculate_dimensions_height(theme->font, text->value_str, scale, length);
        }

        if (align_h->value_int == UI_ALIGN_H_RIGHT) {
            x -= width->value_int;
        } else if (align_h->value_int == UI_ALIGN_H_CENTER) {
            x -= width->value_int / 2;
        }

        if (align_v->value_int == UI_ALIGN_V_TOP) {
            y -= height->value_int;
        } else if (align_v->value_int == UI_ALIGN_V_CENTER) {
            y -= height->value_int / 2;
        }
    }

    int32 start = *index;
    f32 offset_x = (f32) x->value_int;
    f32 offset_y = (f32) y->value_int;

    int32 first_char = is_ascii ? text->value_str[0] : utf8_get_char_at(text->value_str, 0);
    for (int32 i = (first_char == '\n' ? 1 : 0); i < length; ++i) {
        int32 character = is_ascii ? text->value_str[i] : utf8_get_char_at(text->value_str, i);

        if (character == '\n') {
            offset_y += theme->font->line_height * scale;
            offset_x = (f32) x->value_int;

            continue;
        }

        Glyph* glyph = font_glyph_find(theme->font, character);
        if (!glyph) {
            continue;
        }

        f32 offset_y2 = offset_y + glyph->metrics.offset_y * scale;
        offset_x += glyph->metrics.offset_x * scale;

        // @performance Consider to handle whitespaces just by offsetting
        vertex_rect_create(
            vertices, index, zindex,
            offset_x, offset_y2, glyph->metrics.width * scale, glyph->metrics.height * scale, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
            color_index->value_float, glyph->coords.x1, glyph->coords.y1, glyph->coords.x2, glyph->coords.y2
        );

        offset_x += (glyph->metrics.width + glyph->metrics.advance_x) * scale;
    }

    element->vertex_count = *index - start;
    memcpy(element->vertices, vertices + start, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);

    // @todo See todo of vertex_text function
    // @performance use elements->vertices and also cache result in there

    return offset_x;
}

void ui_button_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    UIThemeStyle* theme, UIElement* element
)
{
    // @todo handle different states and ongoing animations
    //      We cannot return early in such cases
    if (element->vertex_count > 0) {
        memcpy(vertices + *index, element->vertices, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);
        return;
    }

    // @performance see comment for setup_theme()

    // Load element data
    HashEntryVoidP* element_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, element->name, element->id);
    UIAttributeGroup* element_group = (UIAttributeGroup *) element_entry->value;

    // Load general style
    UIAttribute* style = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_STYLE);
    HashEntryVoidP* style_entry = NULL;
    UIAttributeGroup* style_group = NULL;

    if (style) {
        style_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, style->value_str);
        style_group = (UIAttributeGroup *) style_entry->value;
    }

    UIAttribute* x;
    UIAttribute* y;

    // Load parent data (for position data)
    UIAttribute* parent = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_PARENT);
    if (parent) {
        HashEntryVoidP* parent_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->hash_map, parent->value_str);
        UIAttributeGroup* parent_group = (UIAttributeGroup *) parent_entry->value;

        x = ui_attribute_from_group(parent_group, UI_ATTRIBUTE_TYPE_POSITION_X);
        y = ui_attribute_from_group(parent_group, UI_ATTRIBUTE_TYPE_POSITION_Y);

        // @question Do we have more values which can be inherited from the parent?
        //      We don't want to inherit implicit stuff like size, background etc. These things should be defined explicitly
    } else {
        x = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_POSITION_X);
        y = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_POSITION_Y);
    }

    UIAttribute* width = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH);
    UIAttribute* height = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT);
    UIAttribute* align_h = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_ALIGN_H);
    UIAttribute* align_v = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_ALIGN_V);
    UIAttribute* text = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_CONTENT);
    UIAttribute* size = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_FONT_SIZE);
    UIAttribute* color_index = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_FONT_COLOR);

    // @todo Above we only handle the default values, what about state dependent values like hover, active?
    //  Simply check the state here and load the child_entries based on the state
    //  However, for that we need the current state of the button... should this be in a separate button object,
    //  that also holds position information for hover checks etc. Or should that state be stored in the theme data?
    //  Right now we could make these checks right here anyway but in the future we don't want to update the rendering data
    //  every frame if we don't have to. We don't want immediate mode! We only want to update the UI if there is a change.
    //  If a change (or state change like hover) triggers a complete update of all elements or just a sub region update
    //  remains TBD

    int32 start = *index;

    vertex_rect_border_create(
        vertices, index, zindex,
        x->value_float, y->value_float, width->value_float, height->value_float, 1, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        12, 0.0f, 0.0f
    );

    vertex_rect_create(
        vertices, index, zindex,
        x->value_float + 1, y->value_float + 1, width->value_float - 2, height->value_float - 2, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
        14, 0.0f, 0.0f
    );

    zindex = nextafterf(zindex, INFINITY);

    vertex_text_create(
        vertices, index, zindex,
        x->value_float, y->value_float, width->value_float, height->value_float, align_h->value_int, align_v->value_int,
        theme->font, text->value_str, size->value_float, color_index->value_float
    );

    element->vertex_count = *index - start;
    memcpy(element->vertices, vertices + start, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);
}


#endif