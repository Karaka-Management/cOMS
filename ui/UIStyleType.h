#ifndef TOS_UI_STYLE_TYPE_H
#define TOS_UI_STYLE_TYPE_H

#include "../stdlib/Types.h"

enum UIStyleType : byte {
    UI_STYLE_TYPE_DEFAULT, // = :visible
    UI_STYLE_TYPE_HIDDEN,
    UI_STYLE_TYPE_ACTIVE, // e.g. input
    UI_STYLE_TYPE_DISABLED, // disabled form elements
    UI_STYLE_TYPE_HOVER, // e.g. button
    UI_STYLE_TYPE_MANUAL,
    UI_STYLE_TYPE_SIZE,
};

constexpr
int32 ui_style_type_to_id(const char* str)
{
    if (str_compare(":hidden", str) == 0) {
        return UI_STYLE_TYPE_HIDDEN;
    } else if (str_compare(":active", str) == 0) {
        return UI_STYLE_TYPE_ACTIVE;
    } else if (str_compare(":diabled", str) == 0) {
        return UI_STYLE_TYPE_DISABLED;
    } else if (str_compare(":hover", str) == 0) {
        return UI_STYLE_TYPE_HOVER;
    } else if (str_compare(":manual", str) == 0) {
        return UI_STYLE_TYPE_MANUAL;
    }

    return -1;
}

#endif