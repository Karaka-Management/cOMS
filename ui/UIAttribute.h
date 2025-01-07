#ifndef TOS_UI_STYLE_H
#define TOS_UI_STYLE_H

#include "../stdlib/Types.h"

struct UIAttribute {
    // Attributes use ids (=type name) instead of strings
    int32 attribute_id;

    union {
        char value_str[32];
        int32 value_int;
        f32 value_float;
        v4_f32 value_v4_f32;
    };
};

struct UIAttributeGroup {
    int32 attribute_size;
    UIAttribute* attributes;
};

enum UIAttributeType {
    UI_ATTRIBUTE_TYPE_TYPE,
    UI_ATTRIBUTE_TYPE_STYLE,

    UI_ATTRIBUTE_TYPE_DIMENSION_X,
    UI_ATTRIBUTE_TYPE_DIMENSION_Y,
    UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH,
    UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT,

    UI_ATTRIBUTE_TYPE_CONTENT,
    UI_ATTRIBUTE_TYPE_CONTENT_ALIGN_H,
    UI_ATTRIBUTE_TYPE_CONTENT_ALIGN_V,

    UI_ATTRIBUTE_TYPE_FONT_NAME,
    UI_ATTRIBUTE_TYPE_FONT_COLOR_INDEX,
    UI_ATTRIBUTE_TYPE_FONT_COLOR,
    UI_ATTRIBUTE_TYPE_FONT_SIZE,
    UI_ATTRIBUTE_TYPE_FONT_WEIGHT,
    UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT,

    UI_ATTRIBUTE_TYPE_ALIGN_H,
    UI_ATTRIBUTE_TYPE_ALIGN_V,

    UI_ATTRIBUTE_TYPE_ZINDEX,

    UI_ATTRIBUTE_TYPE_POSITION_X,
    UI_ATTRIBUTE_TYPE_POSITION_Y,
    UI_ATTRIBUTE_TYPE_PARENT,

    UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR_INDEX,
    UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR,
    UI_ATTRIBUTE_TYPE_BACKGROUND_IMG,
    UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY,
    UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V,
    UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H,
    UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE,

    UI_ATTRIBUTE_TYPE_BORDER_COLOR_INDEX,
    UI_ATTRIBUTE_TYPE_BORDER_COLOR,
    UI_ATTRIBUTE_TYPE_BORDER_WIDTH,
    UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR,
    UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH,
    UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR,
    UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH,
    UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR,
    UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH,
    UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR,
    UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH,

    UI_ATTRIBUTE_TYPE_PADDING,
    UI_ATTRIBUTE_TYPE_PADDING_TOP,
    UI_ATTRIBUTE_TYPE_PADDING_RIGHT,
    UI_ATTRIBUTE_TYPE_PADDING_BOTTOM,
    UI_ATTRIBUTE_TYPE_PADDING_LEFT,

    UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR_INDEX,
    UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR,
    UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE,
    UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE,

    UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR_INDEX,
    UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR,
    UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE,
    UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE,

    // @todo This isn't enough, we can have many animations (position, size, colors, ...)
    //  Maybe we need to define an animation child which overwrites the defined values
    //  Maybe it should use the same system as state dependent values like hover, active, ...
    UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION,
    UI_ATTRIBUTE_TYPE_TRANSITION_DURATION,

    UI_ATTRIBUTE_TYPE_SIZE,
};

UIAttribute* ui_attribute_from_group(UIAttributeGroup* group, UIAttributeType type)
{
    if (!group->attributes) {
        return NULL;
    }

    int32 left = 0;
    int32 right = type;

    // Binary search since attributes are sorted by attribute_id
    while (left <= right) {
        int32 mid = left + (right - left) / 2;

        if (group->attributes[mid].attribute_id == type) {
            return &group->attributes[mid];
        }  else if (group->attributes[mid].attribute_id < type) {
            left = mid + 1;
        }  else {
            right = mid - 1;
        }
    }

    return NULL;
}

constexpr const char* ui_attribute_type_to_string(UIAttributeType e)
{
    switch (e) {
        case UI_ATTRIBUTE_TYPE_TYPE:
            return "type";
        case UI_ATTRIBUTE_TYPE_STYLE:
            return "style";
        case UI_ATTRIBUTE_TYPE_DIMENSION_X:
            return "x";
        case UI_ATTRIBUTE_TYPE_DIMENSION_Y:
            return "y";
        case UI_ATTRIBUTE_TYPE_DIMENSION_WIDTH:
            return "width";
        case UI_ATTRIBUTE_TYPE_DIMENSION_HEIGHT:
            return "height";
        case UI_ATTRIBUTE_TYPE_FONT_NAME:
            return "font_name";
        case UI_ATTRIBUTE_TYPE_FONT_COLOR:
            return "font_color";
        case UI_ATTRIBUTE_TYPE_FONT_SIZE:
            return "font_size";
        case UI_ATTRIBUTE_TYPE_FONT_WEIGHT:
            return "font_weight";
        case UI_ATTRIBUTE_TYPE_FONT_LINE_HEIGHT:
            return "font_line_height";
        case UI_ATTRIBUTE_TYPE_ALIGN_H:
            return "align_h";
        case UI_ATTRIBUTE_TYPE_ALIGN_V:
            return "align_v";
        case UI_ATTRIBUTE_TYPE_ZINDEX:
            return "zindex";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_COLOR:
            return "background_color";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_IMG:
            return "background_img";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_OPACITY:
            return "background_img_opacity";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_V:
            return "background_img_position_v";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_POSITION_H:
            return "background_img_position_h";
        case UI_ATTRIBUTE_TYPE_BACKGROUND_IMG_STYLE:
            return "background_img_style";
        case UI_ATTRIBUTE_TYPE_BORDER_COLOR:
            return "border_color";
        case UI_ATTRIBUTE_TYPE_BORDER_WIDTH:
            return "border_width";
        case UI_ATTRIBUTE_TYPE_BORDER_TOP_COLOR:
            return "border_top_color";
        case UI_ATTRIBUTE_TYPE_BORDER_TOP_WIDTH:
            return "border_top_width";
        case UI_ATTRIBUTE_TYPE_BORDER_RIGHT_COLOR:
            return "border_right_color";
        case UI_ATTRIBUTE_TYPE_BORDER_RIGHT_WIDTH:
            return "border_right_width";
        case UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_COLOR:
            return "border_bottom_color";
        case UI_ATTRIBUTE_TYPE_BORDER_BOTTOM_WIDTH:
            return "border_bottom_width";
        case UI_ATTRIBUTE_TYPE_BORDER_LEFT_COLOR:
            return "border_left_color";
        case UI_ATTRIBUTE_TYPE_BORDER_LEFT_WIDTH:
            return "border_left_width";
        case UI_ATTRIBUTE_TYPE_PADDING:
            return "padding";
        case UI_ATTRIBUTE_TYPE_PADDING_TOP:
            return "padding_top";
        case UI_ATTRIBUTE_TYPE_PADDING_RIGHT:
            return "padding_right";
        case UI_ATTRIBUTE_TYPE_PADDING_BOTTOM:
            return "padding_bottom";
        case UI_ATTRIBUTE_TYPE_PADDING_LEFT:
            return "padding_left";
        case UI_ATTRIBUTE_TYPE_SHADOW_INNER_COLOR:
            return "shadow_inner_color";
        case UI_ATTRIBUTE_TYPE_SHADOW_INNER_ANGLE:
            return "shadow_inner_angle";
        case UI_ATTRIBUTE_TYPE_SHADOW_INNER_DISTANCE:
            return "shadow_inner_distance";
        case UI_ATTRIBUTE_TYPE_SHADOW_OUTER_COLOR:
            return "shadow_outer_color";
        case UI_ATTRIBUTE_TYPE_SHADOW_OUTER_ANGLE:
            return "shadow_outer_angle";
        case UI_ATTRIBUTE_TYPE_SHADOW_OUTER_DISTANCE:
            return "shadow_outer_distance";
        case UI_ATTRIBUTE_TYPE_TRANSITION_ANIMATION:
            return "transition_animation";
        case UI_ATTRIBUTE_TYPE_TRANSITION_DURATION:
            return "transition_duration";
    }

    return NULL;
}

#endif