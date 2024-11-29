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

void image_from_file(RingMemory* ring, const char* path, Image* image)
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

    for (int y = 0; y < image->height; ++y) {
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

#endif