#ifndef TOS_UI_LAYOUT_H
#define TOS_UI_LAYOUT_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../stdlib/HashMap.h"
#include "../asset/Asset.h"
#include "../camera/Camera.h"

#include "../system/FileUtils.cpp"

#include "UITheme.h"
#include "UIElement.h"
#include "UIElementType.h"

#define UI_LAYOUT_VERSION 1

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

    // Used to directly find element by name
    // The values are pointers to the UIElements
    // @todo Should be a perfect hash map
    HashMap hash_map;

    // UI data
    // Only the size of the fixed layout, doesn't include the theme specific data
    uint32 layout_size;

    // Total possible size
    uint32 data_size;

    // Holds the ui elements
    // The structure of the data is as follows:
    //      1. HashMap data (points to 2.a.)
    //      2. UIElements (default)
    //          a. General UIElement
    //          b. Element specific state
    //          c. Element specific style
    //      3. Additional UI styles (see c.), dynamically created when the theme is loaded
    // We effectively create a tree in data where the individual elements can get directly accessed through the hashmap
    byte* data;

    // Changes on a as needed basis
    uint32 vertex_size_static;
    bool static_content_changed;

    // Changes every frame
    uint32 vertex_size_dynamic;
    bool dynamic_content_changed;

    // Contains both static and dynamic content
    // @todo The vertices shouldn't be an Asset, it's more like a ECS, maybe it's not even in RAM and only in VRAM?!
    // One of the reasons for this being an asset is also that it is easy to log ram/vram usage but that can be fixed
    Asset* ui_asset;

    // Total count of the ui_asset vertices
    uint32 vertex_size;

    // @question Should we maybe also hold the font atlas asset here AND the color palette?
};

void count_direct_child_elements(UIElement* element, const char* pos, int32 parent_level)
{
    // Find amount of child elements
    // We have to perform a lookahead since this determins the since this determines the size of our current element
    uint16 direct_child_elements = 0;

    int32 level = 0;
    while (*pos != '\0') {
        while (*pos == ' ' || *pos == '\t') {
            ++pos;
            ++level;
        }

        if (level > parent_level + 4) {
            // This element is a childrens child and not a direct child
            continue;
        } else if (level <= parent_level) {
            // We are no longer inside of element
            break;
        }
    }

    element->children_count = direct_child_elements;
}

void assign_child_elements(UILayout* layout, UIElement* element, char* pos, int32 parent_level) {
    int32 current_child_pos = 0;

    char block_name[28];

    int32 level = 0;
    while (*pos != '\0') {
        while (*pos == ' ' || *pos == '\t') {
            ++pos;
            ++level;
        }

        if (level > parent_level + 4) {
            // This element is a childrens child and not a direct child
            continue;
        } else if (level <= parent_level) {
            // We are no longer inside of element
            break;
        }

        str_copy_move_until(&pos, block_name, ":");
        str_move_past(&pos, '\n');

        element->children[current_child_pos] = (UIElement *) (
            layout->data
            + ((HashEntryInt64 *) hashmap_get_entry(&layout->hash_map, block_name))->value
        );

        element->children[current_child_pos]->parent = element;

        ++current_child_pos;
    }
}

// WARNING: theme needs to have memory already reserved and assigned to data
void layout_from_file_txt(
    UILayout* layout,
    const char* path,
    RingMemory* ring
) {
    FileBody file;
    file_read(path, &file, ring);
    ASSERT_SIMPLE(file.size);

    char* pos = (char *) file.content;

    // move past the version string
    pos += 8;

    // Use version for different handling
    int32 version = strtol(pos, &pos, 10); ++pos;

    // 1. Iteration: We have to find how many elements are defined in the layout file.
    // Therefore we have to do an initial iteration
    int32 temp_element_count = 0;
    while (*pos != '\0') {
        // Skip all white spaces
        str_skip_empty(&pos);

        ++temp_element_count;

        // Go to the next line
        str_move_past(&pos, '\n');
    }

    // 2. Iteration: Fill HashMap
    // @performance This is probably horrible since we are not using a perfect hashing function (1 hash -> 1 index)
    //      I wouldn't be surprised if we have a 50% hash overlap (2 hashes -> 1 index)
    hashmap_create(&layout->hash_map, temp_element_count, sizeof(HashEntryVoidP), layout->data);
    int64 hm_size = hashmap_size(&layout->hash_map);

    pos = (char *) file.content;

    // move past version string
    str_move_past(&pos, '\n');

    char block_name[28];
    char block_type[28];

    byte* element_data = layout->data + hm_size;

    int32 level;
    while (*pos != '\0') {
        if (*pos == '\n') {
            ++pos;
            continue;
        }

        level = 0;
        while (*pos == ' ' || *pos == '\t')  {
            ++pos;
            ++level;
        }

        if (*pos == '\n' || *pos == '\0') {
            continue;
        }

        str_copy_move_until(&pos, block_name, ":"); ++pos;
        str_copy_move_until(&pos, block_type, " \r\n");
        str_move_past(&pos, '\n');

        UIElement* element = (UIElement *) element_data;
        element->type = (UIElementType) ui_element_type_to_id(block_type);
        element->children = (UIElement **) (element_data + sizeof(UIElement));
        count_direct_child_elements(element, pos, level);

        // Every UIElement can have child elements, we need to store the pointers to those elements
        element_data += sizeof(UIElement) + sizeof(UIElement*) * element->children_count;

        // We also put the state data after this element
        element->state = element_data;
        element_data += ui_element_state_size(element->type);

        // We also put the default element data after this element
        // Depending on the theme we will have also additional styles (e.g. :active, :hidden, ...)
        element->details[0] = element_data;

        // @performance We should probably make sure the data is nicely aligned here
        element_data += ui_element_type_size(element->type);

        // Insert new element
        hashmap_insert(&layout->hash_map, block_name, element_data - layout->data);
    }

    // 3. Iteration: Create child references
    pos = (char *) file.content;

    // move past version string
    str_move_past(&pos, '\n');

    while (*pos != '\0') {
        if (*pos == '\n') {
            ++pos;
            continue;
        }

        level = 0;
        while (*pos == ' ' || *pos == '\t')  {
            ++pos;
            ++level;
        }

        if (*pos == '\n' || *pos == '\0') {
            continue;
        }

        str_copy_move_until(&pos, block_name, ":");
        str_move_past(&pos, '\n');

        UIElement* element = (UIElement *) (layout->data + ((HashEntryInt64 *) hashmap_get_entry(&layout->hash_map, block_name))->value);
        assign_child_elements(layout, element, pos, level);
    }
}

static inline
void ui_layout_serialize_element(HashEntryInt64* entry, byte* data, byte** pos, const byte* start)
{
    // @performance Are we sure the data is nicely aligned?
    // Probably depends on the from_txt function and the start of layout->data
    UIElement* element = (UIElement *) (data + entry->value);

    **pos = element->state_flag;
    *pos += sizeof(element->state_flag);

    **pos = element->type;
    *pos += sizeof(element->type);

    **pos = element->style_old;
    *pos += sizeof(element->style_old);

    **pos = element->style_new;
    *pos += sizeof(element->style_new);

    // Parent
    if (element->parent) {
        *((uint64 *) *pos) = SWAP_ENDIAN_LITTLE((uint64) ((uintptr_t) element->parent - (uintptr_t) data));
    } else {
        memset(*pos, 0, sizeof(uint64));
    }

    *pos += sizeof(uint64);

    // State
    if (element->state) {
        *((uint64 *) *pos) = SWAP_ENDIAN_LITTLE((uint64) ((uintptr_t) element->state - (uintptr_t) data));
    } else {
        memset(*pos, 0, sizeof(uint64));
    }

    *pos += sizeof(uint64);

    // Details
    for (int32 j = 0; j < UI_STYLE_TYPE_SIZE; ++j) {
        if (element->details[j]) {
            *((uint64 *) *pos) = SWAP_ENDIAN_LITTLE((uint64) ((uintptr_t) element->details[j] - (uintptr_t) data));
        } else {
            memset(*pos, 0, sizeof(uint64));
        }

        *pos += sizeof(uint64);
    }

    *((uint16 *) *pos) = SWAP_ENDIAN_LITTLE(element->children_count);
    *pos += sizeof(element->children_count);

    for (int32 j = 0; j < element->children_count; ++j) {
        *((uint64 *) *pos) = SWAP_ENDIAN_LITTLE((uint64) ((uintptr_t) element->children[j] - (uintptr_t) data));
        *pos += sizeof(uint64);
    }

    // @performance, shouldn't it just be memset for both elements?
    // state element data e.g. UIInputState
    memcpy(*pos, element->state, ui_element_state_size(element->type));
    *pos += ui_element_state_size(element->type);

    // detailed element data e.g. UIInput
    memcpy(*pos, element->details[0], ui_element_type_size(element->type));
    *pos += ui_element_type_size(element->type);
}

int32 layout_to_data(
    const UILayout* layout,
    byte* data
) {
    byte* pos = data;
    byte* max_pos = data;

    // version
    *((int32 *) pos) = SWAP_ENDIAN_LITTLE(UI_LAYOUT_VERSION);
    pos += sizeof(int32);

    // hashmap
    byte* start = pos;
    pos += hashmap_dump(&layout->hash_map, pos);

    // UIElement data
    for (uint32 i = 0; i < layout->hash_map.buf.count; ++i) {
        if (!layout->hash_map.table[i]) {
            continue;
        }

        HashEntryInt64* entry = (HashEntryInt64 *) layout->hash_map.table[i];

        pos = start + entry->value;
        ui_layout_serialize_element(entry, layout->data, &pos, start);
        if (pos > max_pos) {
            max_pos = pos;
        }

        // save all the next elements
        while (entry->next) {
            pos = start + entry->value;
            ui_layout_serialize_element(entry, layout->data, &pos, start);
            if (pos > max_pos) {
                max_pos = pos;
            }
            entry = entry->next;
        }
    }

    return (int32) (max_pos - data);
}

static inline
void ui_layout_parse_element(HashEntryInt64* entry, byte* data, const byte** pos)
{
    // @performance Are we sure the data is nicely aligned?
    // Probably depends on the from_txt function and the start of layout->data

    // Change offset to pointer
    entry->value = (uintptr_t) data + entry->value;

    UIElement* element = (UIElement *) entry->value;

    element->state_flag = **pos;
    *pos += sizeof(element->state_flag);

    element->type = (UIElementType) **pos;
    *pos += sizeof(element->type);

    element->style_old = (UIStyleType) **pos;
    *pos += sizeof(element->style_old);

    element->style_new = (UIStyleType) **pos;
    *pos += sizeof(element->style_new);

    // Parent
    uint64 temp = SWAP_ENDIAN_LITTLE(*((uint64 *) *pos));
    element->parent = temp
        ? (UIElement *) (data + temp)
        : NULL;

    *pos += sizeof(uint64);

    // State
    temp = SWAP_ENDIAN_LITTLE(*((uint64 *) *pos));
    element->state = temp
        ? data + temp
        : NULL;

    *pos += sizeof(uint64);

    // Details
    for (int32 j = 0; j < UI_STYLE_TYPE_SIZE; ++j) {
        temp = SWAP_ENDIAN_LITTLE(*((uint64 *) *pos));
        element->details[j] = temp
            ? data + temp
            : NULL;

        *pos += sizeof(uint64);
    }

    element->children_count = SWAP_ENDIAN_LITTLE(*((uint16 *) *pos));
    *pos += sizeof(element->children_count);

    for (int32 j = 0; j < element->children_count; ++j) {
        temp = SWAP_ENDIAN_LITTLE(*((uint64 *) *pos));
        element->children[j] = temp
            ? (UIElement *) (data + temp)
            : NULL; // this should never happen since the children_count only gets incremented if it has a child

        if (element->children[j]) {
            element->children[j]->parent = element;
        }

        *pos += sizeof(uint64);
    }

    // @performance, shouldn't it just be memset for both elements?
    // state element data e.g. UIInput
    memcpy(element->state, *pos, ui_element_state_size(element->type));
    *pos += ui_element_state_size(element->type);

    // detailed element data e.g. UIInput
    memcpy(element->details[0], *pos, ui_element_type_size(element->type));
    *pos += ui_element_type_size(element->type);
}

// The size of layout->data should be the file size + a bunch of additional data for additional theme dependent "UIElements->details".
// Yes, this means we have a little too much data but not by a lot
int32 layout_from_data(
    const byte* data,
    UILayout* layout
) {
    const byte* pos = data;
    const byte* max_pos = pos;

    int32 version = *((int32 *) pos);
    pos += sizeof(version);

    // Prepare hashmap (incl. reserve memory) by initializing it the same way we originally did
    // Of course we still need to populate the data using hashmap_load()
    // The value is a int64 (because this is the value of the chunk buffer size but the hashmap only allows int32)
    hashmap_create(&layout->hash_map, (int32) SWAP_ENDIAN_LITTLE(*((uint64 *) pos)), sizeof(HashEntryInt64), layout->data);

    const byte* start = data;
    pos += hashmap_load(&layout->hash_map, pos);

    // layout data
    for (int32 i = 0; i < layout->hash_map.buf.count; ++i) {
        if (!layout->hash_map.table[i]) {
            continue;
        }

        HashEntryInt64* entry = (HashEntryInt64 *) layout->hash_map.table[i];

        pos = start + entry->value;
        ui_layout_parse_element(entry, layout->data, &pos);
        if (pos > max_pos) {
            max_pos = pos;
        }

        // save all the next elements
        while (entry->next) {
            pos = start + entry->value;
            ui_layout_parse_element(entry, layout->data, &pos);
            if (pos > max_pos) {
                max_pos = pos;
            }
            entry = entry->next;
        }
    }

    layout->layout_size = (uint32) (max_pos - data);

    return (int32) layout->layout_size;
}

// @performance Implement a way to only load a specific element and all its children
// This way we can re-load specific elements on change and we could also greatly reduce the setup time by ignoring ui elements that are rarely visible

void layout_from_theme(
    UILayout* layout,
    const UIThemeStyle* theme,
    const Camera* camera
) {
    EvaluatorVariable variables[] = {
        { "vw", (f32) camera->viewport_width },
        { "vh", (f32) camera->viewport_height },
        { "px", 0.0 }, // Placeholder for parent values
        { "py", 0.0 }, // Placeholder for parent values
        { "pw", 0.0 }, // Placeholder for parent values
        { "ph", 0.0 }, // Placeholder for parent values
    };

    // Current position where we can the different sub elements (e.g. :hover, :active, ...)
    byte* dynamic_pos = layout->data + layout->layout_size;

    // We first need to handle the default element -> iterate all elements but only handle the default style
    // @todo do here
    for (int32 i = 0; i < theme->hash_map.buf.count; ++i) {
        if (!theme->hash_map.table[i]) {
            continue;
        }

        HashEntryUIntPtr* style_entry = (HashEntryUIntPtr *) theme->hash_map.table[i];

        // We don't handle special styles here, only the default one
        if (strchr(style_entry->key, ':')) {
            continue;
        }

        HashEntry* entry = hashmap_get_entry(&layout->hash_map, style_entry->key);
        if (!entry) {
            // Couldn't even find the base element
            continue;
        }

        // Populate default element
        UIElement* element = (UIElement *) entry->value;
        UIAttributeGroup* group = (UIAttributeGroup *) style_entry->value;

        switch (element->type) {
            case UI_ELEMENT_TYPE_INPUT: {
                ui_input_state_populate(group, (UIInputState *) element->state);
                ui_input_element_populate(group, element, UI_STYLE_TYPE_DEFAULT, variables);
            } break;
        }
    }

    // We iterate every style
    //      1. Fill default element if it is default style
    //      2. Create and fill new element if it isn't default style (e.g. :hover)
    for (int32 i = 0; i < theme->hash_map.buf.count; ++i) {
        if (!theme->hash_map.table[i]) {
            continue;
        }

        HashEntryUIntPtr* style_entry = (HashEntryUIntPtr *) theme->hash_map.table[i];

        // We only handle special styles here, not the default one
        char* special = strchr(style_entry->key, ':');
        if (!special) {
            // The default element was already handled outside this loop
            continue;
        }

        UIStyleType style_type = (UIStyleType) ui_style_type_to_id(special);

        char pure_name[HASH_MAP_MAX_KEY_LENGTH];
        str_copy_until(style_entry->key, pure_name, ':');

        HashEntry* entry = hashmap_get_entry(&layout->hash_map, pure_name);
        if (!entry) {
            // Couldn't even find the base element
            continue;
        }

        UIElement* element = (UIElement *) entry->value;

        // Doesn't exist (usually the first load, but exists when we resize our window)
        if (!element->details[style_type]) {
            element->details[style_type] = dynamic_pos;
            dynamic_pos += ui_element_type_size(element->type);
        }

        // The style inherits from the default element
        memcpy(element->details[style_type], element->details[0], ui_element_type_size(element->type));

        // Populate element details
        UIAttributeGroup* group = (UIAttributeGroup *) style_entry->value;
        switch (element->type) {
            case UI_ELEMENT_TYPE_INPUT: {
                ui_input_element_populate(group, element, style_type, variables);
            } break;
        }
    }
}

inline
uint32 layout_element_from_location(UILayout* layout, uint16 x, uint16 y)
{
    return layout->ui_chroma_codes[layout->width * y / 4 + x / 4];
}

#endif