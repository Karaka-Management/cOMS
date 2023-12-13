/**
 * Karaka
 *
 * @package   Utils
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef UTILS_COLOR_UTILS_H
#define UTILS_COLOR_UTILS_H

#include <stdio.h>
#include <stdlib.h>

namespace Utils::ColorUtils
{
    typedef struct {
        unsigned char r = 0;
        unsigned char g = 0;
        unsigned char b = 0;
    } RGB;

    inline
    RGB* int_to_rgb(int rgb)
    {
        RGB* result = (RGB*) malloc(1 * sizeof(RGB));

        result->r = rgb & 255;
        result->g = (rgb >> 8) & 255;
        result->b = (rgb >> 16) & 255;

        return result;
    }

    inline
    int rgb_to_int(const RGB* rgb)
    {
        int i = (255 & rgb->r) << 16;
        i    += (255 & rgb->g) << 8;
        i    += (255 & rgb->b);

        return i;
    }

    inline
    int rgb_to_int(char r, char g, char b)
    {
        int i = (255 & r) << 16;
        i    += (255 & g) << 8;
        i    += (255 & b);

        return i;
    }
}

#endif