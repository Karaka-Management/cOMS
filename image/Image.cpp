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

    image->image_settings ^= IMAGE_SETTING_BOTTOM_TO_TOP;
}

inline
int32 image_pixel_size_from_type(byte type)
{
    int32 channel_size = type & IMAGE_SETTING_CHANNEL_4_SIZE ? 4 : 1;
    int32 channel_count = type & IMAGE_SETTING_CHANNEL_COUNT;

    return channel_size * channel_count;
}

inline
int32 image_data_size(const Image* image)
{
    return image->pixel_count * image_pixel_size_from_type(image->image_settings)
        + sizeof(image->width) + sizeof(image->height)
        + sizeof(image->image_settings);
}

inline
uint32 image_header_from_data(const byte* data, Image* image)
{
    const byte* start = data;

    image->width = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(image->width);

    image->height = SWAP_ENDIAN_LITTLE(*((uint32 *) data));
    data += sizeof(image->height);

    image->pixel_count = image->width * image->height;

    image->image_settings = *data;
    data += sizeof(image->image_settings);

    return (int32) (data - start);
}

uint32 image_from_data(const byte* data, Image* image)
{
    const byte* pos = data;
    pos += image_header_from_data(data, image);

    int32 image_size;
    memcpy(image->pixels, pos, image_size = (image_pixel_size_from_type(image->image_settings) * image->pixel_count));
    pos += image_size;

    return (int32) (pos - data);
}

inline
uint32 image_header_to_data(const Image* image, byte* data)
{
    byte* start = data;

    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(image->width);
    data += sizeof(image->width);

    *((uint32 *) data) = SWAP_ENDIAN_LITTLE(image->height);
    data += sizeof(image->height);

    *data = image->image_settings;
    data += sizeof(image->image_settings);

    return (int32) (data - start);
}

uint32 image_to_data(const Image* image, byte* data)
{
    byte* pos = data;
    pos += image_header_to_data(image, data);

    int32 image_size;
    memcpy(pos, image->pixels, image_size = (image_pixel_size_from_type(image->image_settings) * image->pixel_count));
    pos += image_size;

    return (int32) (pos - data);
}

#endif