/**
 * Jingga
 *
 * @copyright Jingga
 * @license   OMS License 2.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef TOS_IMAGE_C
#define TOS_IMAGE_C

#include "../utils/StringUtils.h"
#include "../memory/RingMemory.h"

#if _WIN32
    #include "../platform/win32/FileUtils.cpp"
#else
    #include "../platform/linux/FileUtils.cpp"
#endif

#include "Image.h"
#include "Tga.h"
#include "Bitmap.h"
#include "Png.h"

void image_from_file(Image* image, const char* path, RingMemory* ring)
{
    FileBody file;
    file_read(path, &file, ring);

    if (str_ends_with(path, ".png")) {
        image_png_generate(&file, image, ring);
    } else if (str_ends_with(path, ".tga")) {
        image_tga_generate(&file, image);
    } else if (str_ends_with(path, ".bmp")) {
        image_bmp_generate(&file, image);
    }
}

void image_flip_vertical(RingMemory* ring, Image* image)
{
    uint32 stride = image->width * sizeof(uint32);
    byte* temp = ring_get_memory(ring, image->pixel_count * sizeof(uint32));
    memcpy(temp, image->pixels, image->pixel_count * sizeof(uint32));

    // Last row
    const byte* end = temp + image->pixel_count * sizeof(uint32) - image->width * sizeof(uint32);

    for (uint32 y = 0; y < image->height; ++y) {
        memcpy(image->pixels + y * stride, end - y * stride, stride);
    }

    /* Flipping with small temp row
    byte* temp_row = ring_get_memory(ring, stride);

    for (int y = 0; y < image->height / 2; ++y) {
        memcpy(temp_row, image->pixels + y * stride, stride);

        memcpy(image->pixels + y * stride, image->pixels - y * stride, stride);
        memcpy(image->pixels - y * stride, temp_row, stride);
    }
    */

    image->order_rows = (byte) (!((bool) image->order_rows));
}

inline
int32 image_pixel_size_from_type(byte type)
{
    switch (type) {
        case PIXEL_TYPE_RGBA: {
            return 4;
        } break;
        case PIXEL_TYPE_RGB: {
            return 3;
        } break;
        case PIXEL_TYPE_MONO: {
            return 1;
        } break;
        case PIXEL_TYPE_RGBA_F: {
            return 16;
        } break;
        case PIXEL_TYPE_RGB_F: {
            return 12;
        } break;
        case PIXEL_TYPE_MONO_F: {
            return 4;
        } break;
        default: {
            UNREACHABLE();
        }
    }
}

int32 image_from_data(const byte* data, Image* image)
{
    const byte* pos = data;

    image->width = SWAP_ENDIAN_LITTLE(*((uint32 *) pos));
    pos += sizeof(image->width);

    image->height = SWAP_ENDIAN_LITTLE(*((uint32 *) pos));
    pos += sizeof(image->height);

    image->pixel_count = SWAP_ENDIAN_LITTLE(*((uint32 *) pos));
    pos += sizeof(image->pixel_count);

    image->order_pixels = *pos;
    pos += sizeof(image->order_pixels);

    image->order_rows = *pos;
    pos += sizeof(image->order_rows);

    image->pixel_type = *pos;
    pos += sizeof(image->pixel_type);

    int32 image_size;
    memcpy(image->pixels, pos, image_size = (image_pixel_size_from_type(image->pixel_type) * image->pixel_count));
    pos += image_size;

    return (int32) (pos - data);
}

int32 image_to_data(const Image* image, byte* data)
{
    byte* pos = data;

    *((uint32 *) pos) = SWAP_ENDIAN_LITTLE(image->width);
    pos += sizeof(image->width);

    *((uint32 *) pos) = SWAP_ENDIAN_LITTLE(image->height);
    pos += sizeof(image->height);

    *((uint32 *) pos) = SWAP_ENDIAN_LITTLE(image->pixel_count);
    pos += sizeof(image->pixel_count);

    *pos = image->order_pixels;
    pos += sizeof(image->order_pixels);

    *pos = image->order_rows;
    pos += sizeof(image->order_rows);

    *pos = image->pixel_type;
    pos += sizeof(image->pixel_type);

    int32 image_size;
    memcpy(pos, image->pixels, image_size = (image_pixel_size_from_type(image->pixel_type) * image->pixel_count));
    pos += image_size;

    return (int32) (pos - data);
}

#endif