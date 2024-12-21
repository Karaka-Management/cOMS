/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_GPUAPI_RENDER_UTILS_H
#define TOS_GPUAPI_RENDER_UTILS_H

#include <stdio.h>
#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/StringUtils.h"
#include "../math/matrix/MatrixFloat32.h"
#include "../font/Font.h"
#include "../object/Vertex.h"
#include "../ui/UITheme.h"
#include "../ui/UIElement.h"
#include "../ui/UIAlignment.h"

// @performance Create improved vertex generation for components (input + button, chat, ...) where we don't use as many
//      degenerate triangle

// @todo in many places we use ->value_int. We should load it as a value_float and also define it as float in the theme.
// This way we wouldn't have to convert the value

inline
void vertex_degenerate_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y
) {
    // Degenerate triangles
    // They are alternating every loop BUT since we use references they look the same in code
    // WARNING: Before using we must make sure that the 0 index is defined
    //          The easiest way is to just define a "degenerate" starting point
    vertices[*index] = {{vertices[*index - 1].position.x, vertices[*index - 1].position.y, zindex}, {0, 0}, 0};
    ++(*index);

    vertices[*index] = {{x, y, zindex}, {0, 0}, 0};
    ++(*index);
}

inline
void vertex_line_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x1, f32 y1, f32 x2, f32 y2, f32 thickness, int32 align_h, int32 align_v,
    f32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
) {
    if (align_h == UI_ALIGN_H_RIGHT) {
        x1 -= thickness;
        x2 -= thickness;
    } else if (align_h == UI_ALIGN_H_CENTER) {
        x1 -= thickness / 2;
        x2 -= thickness / 2;
    }

    if (align_v == UI_ALIGN_V_TOP) {
        y1 -= thickness;
        y2 -= thickness;
    } else if (align_v == UI_ALIGN_V_CENTER) {
        y1 -= thickness / 2;
        y2 -= thickness / 2;
    }

    f32 n1 = -(y2 - y1);
    f32 n2 = x2 - x1;
    f32 n_ = oms_rsqrt(n2 * n2 + n1 * n1);
    f32 norm1 = n1 * n_;
    f32 norm2 = n2 * n_;

    vertex_degenerate_create(vertices, index, zindex, x1, y1);

    int32 idx = *index;

    vertices[idx++] = {{x1, y1, zindex}, {tex_x1, tex_y1}, color_index};
    vertices[idx++] = {{x1 + thickness * norm1, y1 + thickness * norm2, zindex}, {tex_x1, tex_y2}, color_index};
    vertices[idx++] = {{x2, y2, zindex}, {tex_x2, tex_y1}, color_index};
    vertices[idx++] = {{x2 + thickness * norm1, y2 + thickness * norm2, zindex}, {tex_x2, tex_y2}, color_index};

    *index = idx;
}

// @performance Do we really want to create the UI as one continuous mesh?
// Individual meshes without degenerates might be faster
inline
void vertex_rect_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    f32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
) {
    if (align_h == UI_ALIGN_H_RIGHT) {
        x -= width;
    } else if (align_h == UI_ALIGN_H_CENTER) {
        x -= width / 2;
    }

    if (align_v == UI_ALIGN_V_TOP) {
        y -= height;
    } else if (align_v == UI_ALIGN_V_CENTER) {
        y -= height / 2;
    }

    vertex_degenerate_create(vertices, index, zindex, x, y);

    f32 y_height = y + height;
    f32 x_width = x + width;

    // Rectangle
    int32 idx = *index;

    vertices[idx++] = {{x, y, zindex}, {tex_x1, tex_y1}, color_index};
    vertices[idx++] = {{x, y_height, zindex}, {tex_x1, tex_y2}, color_index};
    vertices[idx++] = {{x_width, y, zindex}, {tex_x2, tex_y1}, color_index};
    vertices[idx++] = {{x_width, y_height, zindex}, {tex_x2, tex_y2}, color_index};

    *index = idx;
}

inline
void vertex_rect_border_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, f32 thickness, int32 align_h, int32 align_v,
    f32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
) {
    if (align_h == UI_ALIGN_H_RIGHT) {
        x -= width;
    } else if (align_h == UI_ALIGN_H_CENTER) {
        x -= width / 2;
    }

    if (align_v == UI_ALIGN_V_TOP) {
        y -= height;
    } else if (align_v == UI_ALIGN_V_CENTER) {
        y -= height / 2;
    }

    vertex_degenerate_create(vertices, index, zindex, x, y);

    // @bug While this works for the whole rectangle it doesn't work for individual borders
    // @todo We need a version where you can define individual borders

    f32 y_height = y + height;
    f32 y_thickness = y + thickness;
    f32 x_width = x + width;
    f32 x_thickness = x + thickness;

    // Rectangle
    // Top border
    vertices[*index].position.x = x;
    vertices[*index].position.y = y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x;
    vertices[*index].position.y = y_thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x_width;
    vertices[*index].position.y = y;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x_width;
    vertices[*index].position.y = y_thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    // Right border
    vertices[*index].position.x = x_width - thickness;
    vertices[*index].position.y = y_thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x_width;
    vertices[*index].position.y = y_height;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x_width - thickness;
    vertices[*index].position.y = y_height;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    // Bottom border
    vertices[*index].position.x = x_width - thickness;
    vertices[*index].position.y = y_height - thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x;
    vertices[*index].position.y = y_height;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x;
    vertices[*index].position.y = y_height - thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);

    // Left border
    vertices[*index].position.x = x_thickness;
    vertices[*index].position.y = y_height - thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x;
    vertices[*index].position.y = y_thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x1;
    vertices[*index].tex_coord.y = tex_y2;
    vertices[*index].color = color_index;
    ++(*index);

    vertices[*index].position.x = x_thickness;
    vertices[*index].position.y = y_thickness;
    vertices[*index].position.z = zindex;
    vertices[*index].tex_coord.x = tex_x2;
    vertices[*index].tex_coord.y = tex_y1;
    vertices[*index].color = color_index;
    ++(*index);
}

void vertex_input(Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    f32 color_index = 0, f32 tex_x1 = 0.0f, f32 tex_y1 = 0.0f, f32 tex_x2 = 0.0f, f32 tex_y2 = 0.0f
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

static inline
f32 text_calculate_dimensions_height(
    const Font* __restrict font, const char* __restrict text, f32 scale, int32 length
) {
    f32 line_height = font->line_height * scale;
    f32 y = line_height;

    // @todo remember to restrict to width/height if value > 0 -> force width to remain below certain value

    for (int32 i = 0; i < length; ++i) {
        if (text[i] == '\n') {
            y += line_height;
        }
    }

    return y;
}

static inline
f32 text_calculate_dimensions_width(
    const Font* __restrict font, const char* __restrict text, bool is_ascii, f32 scale, int32 length
) {
    f32 x = 0;
    f32 offset_x = 0;

    // @todo remember to restrict to width/height if value > 0 -> force width to remain below certain value

    for (int32 i = 0; i < length; ++i) {
        int32 character = is_ascii ? text[i] : utf8_get_char_at(text, i);

        if (character == '\n') {
            x = OMS_MAX(x, offset_x);
            offset_x = 0;

            continue;
        }

        Glyph* glyph = font_glyph_find(font, character);
        if (!glyph) {
            continue;
        }

        offset_x += (glyph->metrics.width + glyph->metrics.offset_x + glyph->metrics.advance_x) * scale;
    }

    return OMS_MAX(x, offset_x);
}

static inline
void text_calculate_dimensions(
    f32* __restrict width, f32* __restrict height,
    const Font* __restrict font, const char* __restrict text, bool is_ascii, f32 scale, int32 length
) {
    f32 line_height = font->line_height * scale;
    f32 x = 0;
    f32 y = line_height;

    f32 offset_x = 0;

    // @todo remember to restrict to width/height if value > 0 -> force width to remain below certain value

    for (int32 i = 0; i < length; ++i) {
        int32 character = is_ascii ? text[i] : utf8_get_char_at(text, i);

        if (character == '\n') {
            x = OMS_MAX(x, offset_x);
            y += line_height;

            offset_x = 0;

            continue;
        }

        Glyph* glyph = font_glyph_find(font, character);
        if (!glyph) {
            continue;
        }

        offset_x += (glyph->metrics.width + glyph->metrics.offset_x + glyph->metrics.advance_x) * scale;
    }

    *width = OMS_MAX(x, offset_x);
    *height = y;
}

// @todo implement shadow (offset + angle + diffuse) or should this be a shader only thing? if so this would be a problem for us since we are handling text in the same shader as simple shapes
// we might want to implement distance field font atlas
v2_f32 vertex_text_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    f32 x, f32 y, f32 width, f32 height, int32 align_h, int32 align_v,
    const Font* __restrict font, const char* __restrict text, f32 size, f32 color_index = 0
) {
    int32 length = utf8_strlen(text);
    bool is_ascii = (int32) strlen(text) == length;
    f32 scale = size / font->size;

    // If we do a different alignment we need to pre-calculate the width and height
    if (align_h != 0 || align_v != 0) {
        if (align_h != 0 && align_v != 0) {
            text_calculate_dimensions(&width, &height, font, text, is_ascii, scale, length);
        } else if (align_h != 0) {
            width = text_calculate_dimensions_width(font, text, is_ascii, scale, length);
        } else {
            height = text_calculate_dimensions_height(font, text, scale, length);
        }

        if (align_h == UI_ALIGN_H_RIGHT) {
            x -= width;
        } else if (align_h == UI_ALIGN_H_CENTER) {
            x -= width / 2;
        }

        if (align_v == UI_ALIGN_V_TOP) {
            y -= height;
        } else if (align_v == UI_ALIGN_V_CENTER) {
            y -= height / 2;
        }
    }

    f32 offset_x = x;
    for (int32 i = 0; i < length; ++i) {
        int32 character = is_ascii ? text[i] : utf8_get_char_at(text, i);
        if (character == '\n') {
            y -= font->line_height * scale;
            offset_x = x;

            continue;
        }

        Glyph* glyph = font_glyph_find(font, character);
        if (!glyph) {
            continue;
        }

        f32 offset_y = y + glyph->metrics.offset_y * scale;
        offset_x += glyph->metrics.offset_x * scale;

        // @performance Consider to handle whitespaces just by offsetting
        vertex_rect_create(
            vertices, index, zindex,
            offset_x, offset_y, glyph->metrics.width * scale, glyph->metrics.height * scale, UI_ALIGN_H_LEFT, UI_ALIGN_V_BOTTOM,
            color_index, glyph->coords.x1, glyph->coords.y1, glyph->coords.x2, glyph->coords.y2
        );

        offset_x += (glyph->metrics.width + glyph->metrics.advance_x) * scale;
    }

    // @question How and where to cut off text out of view (here or somewhere else)
    //      We could just prepare the entire text here but then decide what to render later?
    // @todo If width or height (usually just width) > 0 we use those values for automatic wrapping
    //      This way we can ensure no overflow easily
    // @todo implement line alignment, currently only total alignment is considered

    return {offset_x, y};
}

// @todo implement shadow (offset + angle + diffuse) or should this be a shader only thing? if so this would be a problem for us since we are handling text in the same shader as simple shapes
// we might want to implement distance field font atlas
f32 ui_text_create(
    Vertex3DTextureColorIndex* __restrict vertices, uint32* __restrict index, f32 zindex,
    UITheme* theme, UIElement* element
) {
    if (element->vertex_count > 0) {
        memcpy(vertices + *index, element->vertices, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);

        return vertices[element->vertex_count - 1].position.x;
    }

    // @performance see comment for setup_theme()

    // Load element data
    HashEntryVoidP* element_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, element->name, element->id);
    UIAttributeGroup* element_group = (UIAttributeGroup *) element_entry->value;

    // Load general style
    UIAttribute* style = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_STYLE);
    HashEntryVoidP* style_entry = NULL;
    UIAttributeGroup* style_group = NULL;

    if (style) {
        style_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, style->value_str);
        style_group = (UIAttributeGroup *) style_entry->value;
    }

    UIAttribute* x;
    UIAttribute* y;

    // Load parent data (for position data)
    UIAttribute* parent = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_PARENT);
    if (parent) {
        HashEntryVoidP* parent_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, parent->value_str);
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
    f32 scale = size->value_float / theme->font.size;

    // If we do a different alignment we need to pre-calculate the width and height
    if (align_h != NULL || align_v != NULL) {
        f32 tmp_width = (f32) width->value_int;
        f32 tmp_height = (f32) height->value_int;

        if (align_h != NULL && align_v != NULL) {
            text_calculate_dimensions(&tmp_width, &tmp_height, &theme->font, text->value_str, is_ascii, scale, length);
        } else if (align_h != NULL) {
            tmp_width = text_calculate_dimensions_width(&theme->font, text->value_str, is_ascii, scale, length);
        } else {
            tmp_height = text_calculate_dimensions_height(&theme->font, text->value_str, scale, length);
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
            offset_y += theme->font.line_height * scale;
            offset_x = (f32) x->value_int;

            continue;
        }

        Glyph* glyph = font_glyph_find(&theme->font, character);
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
    UITheme* theme, UIElement* element
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
    HashEntryVoidP* element_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, element->name, element->id);
    UIAttributeGroup* element_group = (UIAttributeGroup *) element_entry->value;

    // Load general style
    UIAttribute* style = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_STYLE);
    HashEntryVoidP* style_entry = NULL;
    UIAttributeGroup* style_group = NULL;

    if (style) {
        style_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, style->value_str);
        style_group = (UIAttributeGroup *) style_entry->value;
    }

    UIAttribute* x;
    UIAttribute* y;

    // Load parent data (for position data)
    UIAttribute* parent = ui_attribute_from_group(element_group, UI_ATTRIBUTE_TYPE_PARENT);
    if (parent) {
        HashEntryVoidP* parent_entry = (HashEntryVoidP *) hashmap_get_entry(&theme->primary_scene->hash_map, parent->value_str);
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
        &theme->font, text->value_str, size->value_float, color_index->value_float
    );

    element->vertex_count = *index - start;
    memcpy(element->vertices, vertices + start, sizeof(Vertex3DTextureColorIndex) * element->vertex_count);
}

inline
void entity_world_space(f32* world_space, const f32* local_space, const f32* model_mat)
{
    mat4vec4_mult(model_mat, local_space, world_space);
}

inline
void entity_view_space(f32* view_space, const f32* world_space, const f32* view_mat)
{
    mat4vec4_mult(view_mat, world_space, view_space);
}

inline
void entity_clip_space(f32* clip_space, const f32* view_space, const f32* projection_mat)
{
    mat4vec4_mult(projection_mat, view_space, clip_space);
}

inline
void entity_clip_space_mat(f32* result_mat, const f32* model_mat, const f32* view_mat, const f32* projection_mat)
{
    f32 temp[16];
    mat4mat4_mult(projection_mat, view_mat, temp);
    mat4mat4_mult(temp, model_mat, result_mat);
}

/**
 * Create the matrix used to transform from local space to clip space
 *
 * This allows us to transform multiple objects with the same matrix
 *
 * Vclip = Mprojection * Mview * Mmodel * Vlocal
 */
void entity_clip_space_mat_sse(f32* result_mat, const f32* model_mat, const f32* view_mat, const f32* projection_mat)
{
    __m128 temp[4];

    __m128 a[4];
    __m128 b[4];

    a[0] = _mm_load_ps(projection_mat);
    a[1] = _mm_load_ps(&projection_mat[4]);
    a[2] = _mm_load_ps(&projection_mat[8]);
    a[3] = _mm_load_ps(&projection_mat[12]);

    b[0] = _mm_load_ps(view_mat);
    b[1] = _mm_load_ps(&view_mat[4]);
    b[2] = _mm_load_ps(&view_mat[8]);
    b[3] = _mm_load_ps(&view_mat[12]);
    _MM_TRANSPOSE4_PS(b[0], b[1], b[2], b[3]);

    mat4mat4_mult_sse(a, b, temp);

    a[0] = temp[0];
    a[1] = temp[1];
    a[2] = temp[2];
    a[3] = temp[3];

    b[0] = _mm_load_ps(model_mat);
    b[1] = _mm_load_ps(&model_mat[4]);
    b[2] = _mm_load_ps(&model_mat[8]);
    b[3] = _mm_load_ps(&model_mat[12]);
    _MM_TRANSPOSE4_PS(b[0], b[1], b[2], b[3]);

    mat4mat4_mult_sse(a, b, temp);
    _mm_store_ps(&result_mat[0], temp[0]);
    _mm_store_ps(&result_mat[4], temp[1]);
    _mm_store_ps(&result_mat[8], temp[2]);
    _mm_store_ps(&result_mat[12], temp[3]);
}

inline
void entity_clip_space_from_local(f32* clip_space, const f32* local_space, const f32* mat)
{
    mat4vec4_mult(mat, local_space, clip_space);
}

inline
void entity_clip_space_from_local_sse(f32* clip_space, const f32* local_space, const f32* mat)
{
    mat4vec4_mult_sse(mat, local_space, clip_space);
}

/*
inline
void entity_screen_space(f32* screen_space, const f32* clip_space, const f32* viewport_mat)
{
    // @todo implement
}
*/

inline
void entity_world_space_sse(f32* world_space, const f32* local_space, const f32* model_mat)
{
    mat4vec4_mult_sse(model_mat, local_space, world_space);
}

inline
void entity_view_space_sse(f32* view_space, const f32* world_space, const f32* view_mat)
{
    mat4vec4_mult_sse(view_mat, world_space, view_space);
}

inline
void entity_clip_space_sse(f32* clip_space, const f32* view_space, const f32* projection_mat)
{
    mat4vec4_mult_sse(projection_mat, view_space, clip_space);
}

/*
inline
void entity_screen_space_sse(f32* screen_space, const f32* clip_space, const f32* viewport_mat)
{
    // @todo implement
}
*/

inline
void entity_world_space_sse(__m128* world_space, const __m128* local_space, const __m128* model_mat)
{
    mat4vec4_mult_sse(model_mat, local_space, world_space);
}

inline
void entity_view_space_sse(__m128* view_space, const __m128* world_space, const __m128* view_mat)
{
    mat4vec4_mult_sse(view_mat, world_space, view_space);
}

inline
void entity_clip_space_sse(__m128* clip_space, const __m128* view_space, const __m128* projection_mat)
{
    mat4vec4_mult_sse(projection_mat, view_space, clip_space);
}

/*
inline
void entity_screen_space_sse(__m128* screen_space, const __m128* clip_space, const __m128* viewport_mat)
{
    // @todo implement
}
*/

#endif