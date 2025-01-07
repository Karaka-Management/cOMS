#ifndef TOS_UI_THEME_H
#define TOS_UI_THEME_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"
#include "../utils/EndianUtils.h"
#include "../utils/StringUtils.h"
#include "../stdlib/HashMap.h"
#include "../font/Font.h"
#include "../system/FileUtils.cpp"

#include "UIAttribute.h"
#include "UIElementType.h"

#define UI_THEME_VERSION 1

// @question Currently there is some data duplication in here and in the UIElement.
//      Not sure if this is how we want this to be or if we want to change this in the future
// Modified for every scene
// WARNING: Make sure the order of this struct and UITheme is the same for the first elements
//          This allows us to cast between both
struct UIThemeStyle {
    byte* data;

    // A theme may have N named styles
    // The hashmap contains the offset where the respective style can be found
    // @performance Switch to perfect hash map
    HashMap hash_map;
};

// General theme for all scenes
struct UITheme {
    // This one usually remains unchanged unless someone changes the theme
    UIThemeStyle ui_general;

    // This one is scene specific and is loaded with the scene
    // We have a pointer that references the currently active scene
    // The other two elements contain the actual data.
    // This allows us to easily pre-fetch scene styles and the pointer allows us to easily switch between them
    // When loading a new scene we simply use the style that is currently not pointed to by primary_scene
    UIThemeStyle* primary_scene;
    UIThemeStyle ui_scene1;
    UIThemeStyle ui_scene2;

    // @question This basically means we only support 1 font for the UI ?!
    //  This is probably something to re-consider
    Font font;

    // @todo add cursor styles
    // @todo what about ui audio?

    char name[32];
};

// @performance Consider to replace HashEntryInt64 with HashEntryVoidP.
//  This way we wouldn't have to do theme->data + entry->value and could just use entry->value
//  Of course this means during the saving and loading we need to convert to and from offsets
//  The problem is that the actual dumping and loading for that part doesn't happen in the hashmap but in the chunk_memory
//  The chunk_memory doesn't know how the value look like -> cannot do the conversion
inline
UIAttributeGroup* theme_style_group(UIThemeStyle* theme, const char* group_name)
{
    HashEntryInt64* entry = (HashEntryInt64 *) hashmap_get_entry(&theme->hash_map, group_name);
    if (!entry) {
        ASSERT_SIMPLE(false);
        return NULL;
    }

    return (UIAttributeGroup *) (theme->data + entry->value);
}

inline
UIAttributeGroup* theme_style_group(UIThemeStyle* theme, const char* group_name, int32 group_id)
{
    HashEntryInt64* entry = (HashEntryInt64 *) hashmap_get_entry(&theme->hash_map, group_name, group_id);
    if (!entry) {
        ASSERT_SIMPLE(false);
        return NULL;
    }

    return (UIAttributeGroup *) (theme->data + entry->value);
}

int compare_by_attribute_id(const void* a, const void* b) {
    UIAttribute* attr_a = (UIAttribute *) a;
    UIAttribute* attr_b = (UIAttribute *) b;

    return attr_a->attribute_id - attr_b->attribute_id;
}

// File layout - text
// version
// #group_name
//      attributes ...
//      attributes ...
//      attributes ...
// #group_name
//      attributes ...
//      attributes ...
//      attributes ...

// WARNING: theme needs to have memory already reserved and assigned to data
void theme_from_file_txt(
    UIThemeStyle* theme,
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

    bool block_open = false;
    char block_name[32];
    char attribute_name[32];
    bool last_token_newline = false;

    // We have to find how many groups are defined in the theme file.
    // Therefore we have to do an initial iteration
    int32 temp_group_count = 0;
    while (*pos != '\0') {
        // Skip all white spaces
        str_skip_empty(&pos);

        // Is group name
        if (*pos == '#' || *pos == '.') {
            ++temp_group_count;
        }

        // Go to the end of the line
        str_move_to(&pos, '\n');

        // Go to next line
        if (*pos != '\0') {
            ++pos;
        }
    }

    // @performance This is probably horrible since we are not using a perfect hashing function (1 hash -> 1 index)
    //      I wouldn't be surprised if we have a 50% hash overlap (2 hashes -> 1 index)
    hashmap_create(&theme->hash_map, temp_group_count, sizeof(HashEntryInt64), theme->data);
    int64 data_offset = hashmap_size(&theme->hash_map);

    UIAttributeGroup* temp_group = NULL;

    pos = (char *) file.content;

    // move past version string
    str_move_past(&pos, '\n');

    while (*pos != '\0') {
        str_skip_whitespace(&pos);

        if (*pos == '\n') {
            ++pos;

            // 2 new lines => closing block
            if (last_token_newline) {
                block_open = false;
                last_token_newline = false;
            } else {
                last_token_newline = true;
            }

            continue;
        }

        last_token_newline = false;

        if (!block_open) {
            str_copy_move_until(&pos, block_name, " \n", sizeof(" \n") - 1);

            // All blocks need to start with #. In the past this wasn't the case and may not be in the future. This is why we keep this if here.
            if (*block_name == '#' || *block_name == '.') {
                // Named style
                block_open = true;

                if (temp_group) {
                    // Before we insert a new group we have to sort the attributes
                    // since this makes searching them later on more efficient.
                    qsort(temp_group->attributes, temp_group->attribute_size, sizeof(UIAttribute), compare_by_attribute_id);
                }

                // Insert new group
                hashmap_insert(&theme->hash_map, block_name, data_offset);

                temp_group = (UIAttributeGroup *) (theme->data + data_offset);
                temp_group->attribute_size = 0;
                temp_group->attributes = (UIAttribute *) (theme->data + data_offset + sizeof(UIAttributeGroup));
                data_offset += sizeof(UIAttributeGroup);
            }

            continue;
        }

        str_copy_move_until(&pos, attribute_name, " :\n", sizeof(" :\n") - 1);

        // Skip any white spaces or other delimeter
        str_skip_list(&pos, " \t:", sizeof(" \t:") - 1);

        ASSERT_SIMPLE((*pos != '\0' && *pos != '\n'));

        // Handle different attribute types
        UIAttribute attribute = {};
        if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_TYPE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_TYPE;

            char str[32];
            str_copy_move_until(&pos, str, '\n');

            for (int32 j = 0; j < UI_ELEMENT_TYPE_SIZE; ++j) {
                if (strcmp(str, ui_element_type_to_string((UIElementType) j)) == 0) {

                    attribute.value_int = j;
                    break;
                }
            }

            ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_STYLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_STYLE;

            str_copy_move_until(&pos, attribute.value_str, '\n');
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_SIZE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_SIZE;
            attribute.value_float = strtof(pos, &pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_WEIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_WEIGHT;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT;
            attribute.value_float = strtof(pos, &pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ALIGN_H), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ALIGN_H;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ALIGN_V), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ALIGN_V;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ZINDEX), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ZINDEX;
            attribute.value_float = SWAP_ENDIAN_LITTLE(strtof(pos, &pos));
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG;

            str_copy_move_until(&pos, attribute.value_str, '\n');
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY;
            attribute.value_float = SWAP_ENDIAN_LITTLE(strtof(pos, &pos));
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_TOP), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_TOP;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_RIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_RIGHT;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_BOTTOM), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_BOTTOM;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_LEFT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_LEFT;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE;
            attribute.value_float = strtof(pos, &pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR;
            hexstr_to_rgba(&attribute.value_v4_f32, pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE;
            attribute.value_float = strtof(pos, &pos);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION;
            attribute.value_int = strtoul(pos, &pos, 10);
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_TRANSITION_DURATION), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_TRANSITION_DURATION;
            attribute.value_float = strtof(pos, &pos);
        } else {
            str_move_to(&pos, '\n');

            continue;
        }

        // Again, currently this if check is redundant but it wasn't in the past and we may need it again in the future.
        if (block_name[0] == '#' || block_name[0] == '.') {
            // Named block
            memcpy(
                temp_group->attributes + temp_group->attribute_size,
                &attribute,
                sizeof(attribute)
            );

            data_offset += sizeof(attribute);
            ++temp_group->attribute_size;
        }

        str_move_to(&pos, '\n');
    }

    // We still need to sort the last group
    qsort(temp_group->attributes, temp_group->attribute_size, sizeof(UIAttribute), compare_by_attribute_id);
}

// Memory layout (data) - This is not the layout of the file itself, just how we represent it in memory
// Hashmap
// UIAttributeGroup - This is where the pointers point to (or what the offset represents)
//      size
//      Attributes ...
//      Attributes ...
//      Attributes ...
// UIAttributeGroup
//      size
//      Attributes ...
//      Attributes ...
//      Attributes ...

// The size of theme->data should be the file size.
// Yes, this means we have a little too much data but not by a lot
int32 theme_from_data(
    const byte* data,
    UIThemeStyle* theme
) {
    const byte* pos = data;

    int32 version = *((int32 *) pos);
    pos += sizeof(version);

    // Prepare hashmap (incl. reserve memory) by initializing it the same way we originally did
    // Of course we still need to populate the data using hashmap_load()
    // The value is a int64 (because this is the value of the chunk buffer size but the hashmap only allows int32)
    hashmap_create(&theme->hash_map, (int32) SWAP_ENDIAN_LITTLE(*((uint64 *) pos)), sizeof(HashEntryInt64), theme->data);

    const byte* start = theme->hash_map.buf.memory;
    pos += hashmap_load(&theme->hash_map, pos);

    // theme data
    // Layout: first load the size of the group, then load the individual attributes
    for (int32 i = 0; i < theme->hash_map.buf.count; ++i) {
        if (!theme->hash_map.table[i]) {
            continue;
        }

        HashEntryInt64* entry = (HashEntryInt64 *) theme->hash_map.table[i];

        pos = start + entry->value;
        UIAttributeGroup* group = (UIAttributeGroup *) (theme->data + entry->value);

        group->attribute_size = SWAP_ENDIAN_LITTLE(*((int32 *) pos));
        pos += sizeof(group->attribute_size);

        // @performance The UIAttribute contains a char array which makes this WAY larger than it needs to be in 99% of the cases
        memcpy(group->attributes, pos, group->attribute_size * sizeof(UIAttribute));
        pos += group->attribute_size * sizeof(UIAttribute);

        // load all the next elements
        while (entry->next) {
            pos = start + entry->value;
            group = (UIAttributeGroup *) (theme->data + entry->value);

            group->attribute_size = SWAP_ENDIAN_LITTLE(*((int32 *) pos));
            pos += sizeof(group->attribute_size);

            // @performance The UIAttribute contains a char array which makes this WAY larger than it needs to be in 99% of the cases
            memcpy(group->attributes, pos, group->attribute_size * sizeof(UIAttribute));
            pos += group->attribute_size * sizeof(UIAttribute);

            entry = entry->next;
        }
    }

    return (int32) (pos - data);
}

// Calculates the maximum theme size
// Not every group has all the attributes (most likely only a small subset)
// However, an accurate calculation is probably too slow and not needed most of the time
inline
int64 theme_data_size(const UIThemeStyle* theme)
{
    return hashmap_size(&theme->hash_map)
        + theme->hash_map.buf.count * UI_ATTRIBUTE_TYPE_SIZE * sizeof(UIAttribute);
}

// File layout - binary
// version
// hashmap size
// Hashmap (includes offsets to the individual groups)
// #group_name (this line doesn't really exist in file, it's more like the pointer offset in the hashmap)
//      size
//      attributes ...
//      attributes ...
//      attributes ...
// #group_name
//      size
//      attributes ...
//      attributes ...
//      attributes ...

int32 theme_to_data(
    const UIThemeStyle* theme,
    byte* data
) {
    byte* pos = data;

    // version
    *((int32 *) pos) = SWAP_ENDIAN_LITTLE(UI_THEME_VERSION);
    pos += sizeof(int32);

    // hashmap
    byte* start = pos;
    pos += hashmap_dump(&theme->hash_map, pos);

    // theme data
    // Layout: first save the size of the group, then save the individual attributes
    for (uint32 i = 0; i < theme->hash_map.buf.count; ++i) {
        if (!theme->hash_map.table[i]) {
            continue;
        }

        HashEntryInt64* entry = (HashEntryInt64 *) theme->hash_map.table[i];

        pos = start + entry->value;
        UIAttributeGroup* group = (UIAttributeGroup *) (theme->data + entry->value);

        *((int32 *) pos) = SWAP_ENDIAN_LITTLE(group->attribute_size);
        pos += sizeof(group->attribute_size);

        // @performance The UIAttribute contains a char array which makes this WAY larger than it needs to be in 99% of the cases
        memcpy(pos, group->attributes, group->attribute_size * sizeof(UIAttribute));
        pos += sizeof(UIAttribute);

        // save all the next elements
        while (entry->next) {
            pos = start + entry->value;
            group = (UIAttributeGroup *) (theme->data + entry->value);

            *((int32 *) pos) = SWAP_ENDIAN_LITTLE(group->attribute_size);
            pos += sizeof(group->attribute_size);

            // @performance The UIAttribute contains a char array which makes this WAY larger than it needs to be in 99% of the cases
            memcpy(pos, group->attributes, group->attribute_size * sizeof(UIAttribute));
            pos += sizeof(UIAttribute);

            entry = entry->next;
        }
    }

    return (int32) (pos - data);
}

#endif