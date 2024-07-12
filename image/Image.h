/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_IMAGE_H
#define TOS_IMAGE_H

#include "../stdlib/Types.h"

#define IMAGE_PIXEL_ORDER_RGBA 0
#define IMAGE_PIXEL_ORDER_BGRA 1

#define IMAGE_ROW_ORDER_TOP_TO_BOTTOM 0
#define IMAGE_ROW_ORDER_BOTTOM_TO_TOP 1

struct Image {
    uint32 width;
    uint32 height;
    uint32 length;

    bool has_alpha;
    byte order_pixels; // RGBA vs BGRA
    byte order_rows; // top-to-bottom vs bottom-to-top

    uint32* pixels;
};

#endif