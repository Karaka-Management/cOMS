#ifndef TOS_UI_ATTRIBUTE_BORDER_H
#define TOS_UI_ATTRIBUTE_BORDER_H

#include "../../stdlib/Types.h"

struct UIAttributeBorder {
    // 4 bits per side
    uint16 thickness;
    uint32 color;
};

#endif