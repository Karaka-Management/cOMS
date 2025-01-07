#ifndef TOS_UI_ELEMENT_TYPE_H
#define TOS_UI_ELEMENT_TYPE_H

#include <stdio.h>

enum UIElementType {
    UI_ELEMENT_TYPE_BUTTON,
    UI_ELEMENT_TYPE_SELECT,
    UI_ELEMENT_TYPE_DROPDOWN,
    UI_ELEMENT_TYPE_TEXTFIELD,
    UI_ELEMENT_TYPE_TEXTAREA,
    UI_ELEMENT_TYPE_IMAGE,
    UI_ELEMENT_TYPE_TEXT,
    UI_ELEMENT_TYPE_LINK,
    UI_ELEMENT_TYPE_TABLE,

    UI_ELEMENT_TYPE_VIEW_WINDOW,
    UI_ELEMENT_TYPE_VIEW_PANEL,
    UI_ELEMENT_TYPE_VIEW_TAB,

    UI_ELEMENT_TYPE_SIZE,
};

constexpr const char* ui_element_type_to_string(UIElementType e)
{
    switch (e) {
        case UI_ELEMENT_TYPE_BUTTON:
            return "button";
        case UI_ELEMENT_TYPE_SELECT:
            return "select";
        case UI_ELEMENT_TYPE_DROPDOWN:
            return "dropdown";
        case UI_ELEMENT_TYPE_TEXTFIELD:
            return "textfield";
        case UI_ELEMENT_TYPE_TEXTAREA:
            return "textarea";
        case UI_ELEMENT_TYPE_IMAGE:
            return "image";
        case UI_ELEMENT_TYPE_TEXT:
            return "text";
        case UI_ELEMENT_TYPE_LINK:
            return "link";
        case UI_ELEMENT_TYPE_TABLE:
            return "table";
        case UI_ELEMENT_TYPE_VIEW_WINDOW:
            return "view_window";
        case UI_ELEMENT_TYPE_VIEW_PANEL:
            return "view_panel";
        case UI_ELEMENT_TYPE_VIEW_TAB:
            return "view_tab";
    }

    return NULL;
}

#endif