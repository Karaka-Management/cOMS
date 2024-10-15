#ifndef TOS_UI_THEME_H
#define TOS_UI_THEME_H

#include "../stdlib/Types.h"
#include "../memory/RingMemory.h"
#include "../utils/EndianUtils.h"
#include "../stdlib/HashMap.h"
#include "../font/Font.h"

#include "UIAttribute.h"
#include "UIElementType.h"

#if _WIN32
    #include "../platform/win32/UtilsWin32.h"
#else
    #include "../platform/linux/UtilsLinux.h"
#endif

#define UI_THEME_VERSION 1

struct UITheme {
    byte* data;

    int32 version;
    char name[32];
    bool is_active;

    // Every element has all the style attributes
    UIAttribute styles_global[UI_ELEMENT_TYPE_SIZE * UI_ATTRIBUTE_TYPE_SIZE];

    // A theme may have N named styles
    // @todo We should probably create a hashmap
    //      key = name
    //      value = pointer to group (in the file we store the offset when we load it into memory convert it to pointer)
    HashMap hash_map;

    int32 style_group_count;
    UIAttributeGroup* style_groups;

    Font font;

    // @todo add cursor styles
};

// WARNING: theme needs to have memory already reserved and asigned to data
void theme_from_file_txt(
    RingMemory* ring,
    const char* path,
    UITheme* theme
) {
    FileBody file;
    file_read(path, &file, ring);

    char* pos = (char *) file.content;
    theme->version = strtol(pos, &pos, 10); ++pos;

    bool block_open = false;
    char block_name[32];
    char attribute_name[32];
    bool last_token_newline = false;

    while (*pos != '\0') {
        while (*pos == ' ' || *pos == '\t') {
            ++pos;
        }

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
            int32 i = 0;
            while (*pos != '\0' && *pos != ' ' && *pos != '\n' && i < 31) {
                block_name[i] = *pos;
                ++pos;
                ++i;
            }

            block_name[i] = '\0';

            if (*block_name == '#') {
                // Named style
                // @todo create new style
            }

            continue;
        }

        int32 i = 0;
        while (*pos != '\0' && *pos != ' ' && *pos != '\n' && i < 31) {
            attribute_name[i] = *pos;
            ++pos;
            ++i;
        }

        attribute_name[i] = '\0';

        // Skip any white spaces or other delimeters
        while (*pos == ' ' || *pos == ':') {
            ++pos;
        }

        ASSERT_SIMPLE((*pos != '\0' && *pos != '\n'));

        // Handle different attribute types
        UIAttribute attribute;
        if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_SIZE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_SIZE;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_WEIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_WEIGHT;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ALIGN_H), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ALIGN_H;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ALIGN_V), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ALIGN_V;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_ZINDEX), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_ZINDEX;
            attribute.value_float = SWAP_ENDIAN_LITTLE(strtof(pos, &pos)); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG;

            i = 0;
            while (*pos != '\0' && *pos != '\n') {
                attribute.value_str[i] = *pos++;
            }

            attribute.value_str[i] = '\0';
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY;
            attribute.value_float = SWAP_ENDIAN_LITTLE(strtof(pos, &pos)); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_TOP), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_TOP;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_RIGHT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_RIGHT;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_BOTTOM), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_BOTTOM;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_PADDING_LEFT), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_PADDING_LEFT;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE;
            attribute.value_float = strtof(pos, &pos); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR), attribute_name) == 0) {
            ++pos; // Skip '#'

            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR;
            uint32 value = (uint32) strtoul(pos, &pos, 16);
            pos += 4;

            attribute.value_v4_f32.r = (f32) ((value >> 24) & 0xFF) / 255.0f;
            attribute.value_v4_f32.g = (f32) ((value >> 16) & 0xFF) / 255.0f;
            attribute.value_v4_f32.b = (f32) ((value >> 8) & 0xFF) / 255.0f;
            attribute.value_v4_f32.a = (f32) (value & 0xFF) / 255.0f;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE;
            attribute.value_float = strtof(pos, &pos); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION;
            attribute.value_int = strtoul(pos, &pos, 10); ++pos;
        } else if (strcmp(ui_attribute_type_to_string(UI_ATTRIBUTE_TYPE_TRANSITION_DURATION), attribute_name) == 0) {
            attribute.attribute_id = UI_ATTRIBUTE_TYPE_TRANSITION_DURATION;
            attribute.value_float = strtof(pos, &pos); ++pos;
        } else {
            continue;
        }

        if (block_name[0] == '#') {
            // Named block
        } else {
            // Global default elements and their attributes
            int32 element_type = -1;
            for (int j = 0; j < UI_ELEMENT_TYPE_SIZE; ++j) {
                if (strcmp(ui_element_type_to_string((UIElementType) j), block_name) == 0) {
                    element_type = j;
                    break;
                }
            }

            if (element_type < 0) {
                continue;
            }

            memcpy(
                theme->styles_global + element_type * UI_ATTRIBUTE_TYPE_SIZE,
                &attribute,
                sizeof(attribute)
            );
        }
    }
}

void theme_from_file(
    RingMemory* ring,
    const char* path,
    UITheme* theme
) {
    // version
    // global definitions
    // names count
    //
}

void theme_to_file(RingMemory* ring,
    const char* path,
    const UITheme* theme
) {

}

#endif