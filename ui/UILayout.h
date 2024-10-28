#ifndef TOS_UI_LAYOUT_H
#define TOS_UI_LAYOUT_H

#include "../stdlib/Types.h"
#include "../stdlib/HashMap.h"
#include "UIElement.h"

// Modified for every scene
struct UILayout {
    // This array has the size of the game window and represents in color codes where interactible ui elements are
    // Size is based on screen size (we don't need full screen size since we assume an interactible element is at least 4 pixels width and height)
    //      width = 25% of screen size
    //      height = 25% of screen size
    uint16 width;
    uint16 height;

    // Contains all UI elements also dynamic ones (e.g. movable windows)
    uint32* ui_chroma_codes;

    // Contains constant UI elements that usually don't change (e.g. HUD)
    uint32* ui_chroma_codes_static;

    // Used to directly find element by name
    // The values are the UIElements
    HashMap hash_map;

    int32 vertex_size;
    int32 vertex_pos;
    Vertex3DTextureColorIndex* vertices;
};

inline
uint32 layout_element_from_location(UILayout* layout, uint16 x, uint16 y)
{
    return layout->ui_chroma_codes[layout->width * y / 4 + x / 4];
}

// This function should only get called if the location of a UI Element changes
// @performance How to handle moving elements (= dragging a window). We don't want to update this while dragging!
void layout_chroma_codes_update(UILayout* layout)
{
    // Reset all
    memcpy(layout->ui_chroma_codes, layout->ui_chroma_codes_static, layout->width * layout->height * sizeof(uint32));

    // @question Are the dimension values below even absolute? They may be in relation to the parent?!
    for (int32 i = 0; i < layout->hash_map.buf.count; ++i) {
        if (!layout->hash_map.table[i]) {
            continue;
        }

        HashEntry* entry = (HashEntry *) layout->hash_map.table[i];
        UIElement* element = (UIElement *) entry->value;

        if (element->is_dynamic) {
            continue;
        }

        int32 y_start = element->dimension.y1 / 4;
        int32 y_end = element->dimension.y2 / 4;
        int32 x_start = element->dimension.x1 / 4;
        int32 x_end = element->dimension.x2 / 4;

        for (int32 y = y_start; y < y_end; ++y) {
            int32 y_offset = layout->width * y;
            for (int32 x = x_start; x < x_end; ++x) {
                layout->ui_chroma_codes[y_offset + x] = (uint32) element->id;
            }
        }

        // Now handle all next elements
        while (entry->next) {
            entry = entry->next;

            element = (UIElement *) entry->value;

            y_start = element->dimension.y1 / 4;
            y_end = element->dimension.y2 / 4;
            x_start = element->dimension.x1 / 4;
            x_end = element->dimension.x2 / 4;

            for (int32 y = y_start; y < y_end; ++y) {
                int32 y_offset = layout->width * y;
                for (int32 x = x_start; x < x_end; ++x) {
                    layout->ui_chroma_codes[y_offset + x] = (uint32) element->id;
                }
            }
        }
    }
}

void layout_chroma_codes_update_static(UILayout* layout)
{
    // Reset all
    memset(layout->ui_chroma_codes_static, 0, layout->width * layout->height * sizeof(uint32));

    // @question Are the dimension values below even absolute? They may be in relation to the parent?!
    for (int32 i = 0; i < layout->hash_map.buf.count; ++i) {
        if (!layout->hash_map.table[i]) {
            continue;
        }

        HashEntry* entry = (HashEntry *) layout->hash_map.table[i];
        UIElement* element = (UIElement *) entry->value;

        if (!element->is_dynamic) {
            continue;
        }

        int32 y_start = element->dimension.y1 / 4;
        int32 y_end = element->dimension.y2 / 4;
        int32 x_start = element->dimension.x1 / 4;
        int32 x_end = element->dimension.x2 / 4;

        for (int32 y = y_start; y < y_end; ++y) {
            int32 y_offset = layout->width * y;
            for (int32 x = x_start; x < x_end; ++x) {
                layout->ui_chroma_codes_static[y_offset + x] = (uint32) element->id;
            }
        }

        // Now handle all next elements
        while (entry->next) {
            entry = entry->next;

            element = (UIElement *) entry->value;

            y_start = element->dimension.y1 / 4;
            y_end = element->dimension.y2 / 4;
            x_start = element->dimension.x1 / 4;
            x_end = element->dimension.x2 / 4;

            for (int32 y = y_start; y < y_end; ++y) {
                int32 y_offset = layout->width * y;
                for (int32 x = x_start; x < x_end; ++x) {
                    layout->ui_chroma_codes_static[y_offset + x] = (uint32) element->id;
                }
            }
        }
    }
}

#endif