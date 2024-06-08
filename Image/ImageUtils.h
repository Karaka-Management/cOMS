/**
 * Jingga
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef IMAGE_IMAGE_UTILS_H
#define IMAGE_IMAGE_UTILS_H

#include <math.h>
#include <stdio.h>

namespace Image::ImageUtils
{
    inline float lightnessFromRgb(int r, int g, int b)
    {
        float vR = (float) r / 255.0f;
        float vG = (float) g / 255.0f;
        float vB = (float) b / 255.0f;

        float lR = vR <= 0.04045 ? vR / 12.92 : powf(((vR + 0.055f) / 1.055f), 2.4f);
        float lG = vG <= 0.04045 ? vG / 12.92 : powf(((vG + 0.055f) / 1.055f), 2.4f);
        float lB = vB <= 0.04045 ? vB / 12.92 : powf(((vB + 0.055f) / 1.055f), 2.4f);

        float y     = 0.2126 * lR + 0.7152 * lG + 0.0722 * lB;
        float lStar = y <= 216.0 / 24389.0 ? y * 24389.0 / 27.0 : powf(y, (1.0 / 3.0)) * 116.0 - 16.0;

        return lStar / 100.0f;
    }
} // namespace Image::ImageUtils

#endif