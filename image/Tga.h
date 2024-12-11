/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_IMAGE_TGA_H
#define TOS_IMAGE_TGA_H

#include <string.h>
#include "../stdlib/Types.h"
#include "../utils/Utils.h"
#include "../utils/EndianUtils.h"
#include "Image.h"

// See: https://en.wikipedia.org/wiki/Truevision_TGA
// IMPORTANT: Remember that we are not using packing for the headers
//      Because of that the struct size is different from the actual header size in the file
//      This means we have to manually asign the data to the headers

// Packed header size
#define TGA_HEADER_SIZE 18
struct TgaHeader {
    byte id_length;
    byte color_map_type;
    byte image_type;
    uint16 color_map_index;
    uint16 color_map_length;
    uint16 color_map_bits;
    uint16 x_origin;
    uint16 y_origin;
    uint16 width;
    uint16 height;
    byte bits_per_pixel;
    byte alpha_bits_per_pixel;
    byte horizonal_ordering;
    byte vertical_ordering;
};

struct Tga {
    TgaHeader header;

    byte* pixels; // WARNING: This is not the owner of the data. The owner is the FileBody

    uint32 size;
    byte* data; // WARNING: This is not the owner of the data. The owner is the FileBody
};

void generate_default_tga_references(const FileBody* file, Tga* tga)
{
    tga->size = (uint32) file->size;
    tga->data = file->content;

    if (tga->size < TGA_HEADER_SIZE) {
        // This shouldn't happen
        return;
    }

    tga->header.id_length = file->content[0];
    tga->header.color_map_type = file->content[1];
    tga->header.image_type = file->content[2];
    tga->header.color_map_index = SWAP_ENDIAN_LITTLE(*((uint16 *) (file->content + 3)));
    tga->header.color_map_length = SWAP_ENDIAN_LITTLE(*((uint16 *) (file->content + 5)));
    tga->header.color_map_bits = file->content[7];
    tga->header.width = SWAP_ENDIAN_LITTLE(*((uint16 *) (file->content + 12)));
    tga->header.height = SWAP_ENDIAN_LITTLE(*((uint16 *) (file->content + 14)));
    tga->header.bits_per_pixel = file->content[16];
    tga->header.alpha_bits_per_pixel = file->content[17] & 0x07;
    tga->header.horizonal_ordering = file->content[17] & (1 << 4); // 0 = left-to-right
    tga->header.vertical_ordering = (file->content[17] & (1 << 5)) == 0 ? 1 : 0; // 0 = top-to-bottom

    tga->pixels = file->content + TGA_HEADER_SIZE
        + tga->header.id_length // can be 0
        + tga->header.color_map_length * (tga->header.color_map_bits / 8); // can be 0
}

void image_tga_generate(const FileBody* src_data, Image* image)
{
    // @performance We are generating the struct and then filling the data.
    //      There is some asignment/copy overhead
    Tga src = {};
    generate_default_tga_references(src_data, &src);

    image->width = src.header.width;
    image->height = src.header.height;
    image->pixel_count = image->width * image->height;

    uint32 pixel_bytes = src.header.bits_per_pixel / 8;
    byte alpha_offset = pixel_bytes > 3;

    if (pixel_bytes == 4) {
        image->pixel_type = (byte) PIXEL_TYPE_RGBA;
    } else if (pixel_bytes == 3) {
        image->pixel_type = (byte) PIXEL_TYPE_RGB;
    } else {
        ASSERT_SIMPLE(false);
    }

    // We can check same settings through equality since we use the same values
    if (image->order_rows == src.header.vertical_ordering
        && image->order_pixels == src.header.horizonal_ordering
    ) {
        // @bug This doesn't consider the situation where we want alpha as a setting but the img doesn't have it
        memcpy((void *) image->pixels, src.pixels, image->pixel_count * pixel_bytes);

        return;
    }

    uint32 pixel_rgb_bytes = pixel_bytes - alpha_offset;
    uint32 width_pixel_bytes = src.header.width * pixel_bytes;

    for (uint32 y = 0; y < src.header.height; ++y) {
        uint32 row_pos1 = y * image->width * pixel_bytes;

        uint32 row_pos2;
        if ((image->order_rows == IMAGE_ROW_ORDER_TOP_TO_BOTTOM && src.header.vertical_ordering == 1)
            || (image->order_rows == IMAGE_ROW_ORDER_BOTTOM_TO_TOP && src.header.vertical_ordering == 0)
        ) {
            row_pos2 = (src.header.height - y - 1) * image->width * pixel_bytes;
        } else {
            row_pos2 = y * width_pixel_bytes;
        }

        for (uint32 x = 0; x < src.header.width; ++x) {
            if (image->order_pixels == src.header.horizonal_ordering) {
                for (uint32 i = 0; i < pixel_rgb_bytes; ++i) {
                    image->pixels[row_pos1 + x * pixel_bytes + i] = src.pixels[row_pos2 + x * pixel_bytes + i];
                }
            } else {
                for (uint32 i = 0; i < pixel_rgb_bytes; ++i) {
                    image->pixels[row_pos1 + x * pixel_bytes + i] = src.pixels[row_pos2 + x * pixel_bytes + pixel_rgb_bytes - i];
                }
            }

            // Add alpha channel at end of every RGB value
            if (alpha_offset > 0) {
                image->pixels[row_pos1 + x * pixel_bytes + 3] = src.pixels[row_pos2 + x * pixel_bytes + pixel_bytes + 3];
            } else if (image->pixel_type == PIXEL_TYPE_RGBA) {
                image->pixels[row_pos1 + x * pixel_bytes + 3] = 0xFF;
            }
        }
    }
}

#endif