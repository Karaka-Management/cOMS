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

// This struct also functions as a setting on how to load the image data
//      has_alpha is defined it forces an alpha channel even for bitmaps
//      order_pixels defines how the pixels should be ordered
//      order_rows defines how the rows should be ordered
struct Image {
    uint32 width;
    uint32 height;
    uint32 pixel_count; // @question Do we even need this?

    // Image settings
    bool has_alpha;
    byte order_pixels; // RGBA vs BGRA
    byte order_rows; // top-to-bottom vs bottom-to-top

    uint32* pixels; // owner of data
};

#endif