/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef IMAGE_IMAGE_UTILS_H
#define IMAGE_IMAGE_UTILS_H

#include <stdio.h>
#include <math.h>

namespace Image
{
    namespace ImageUtils
    {
        inline
        float lightnessFromRgb(int r, int g, int b)
        {
            float vR = r / 255.0;
            float vG = g / 255.0;
            float vB = b / 255.0;

            float lR = vR <= 0.04045 ? vR / 12.92 : pow(((vR + 0.055) / 1.055), 2.4);
            float lG = vG <= 0.04045 ? vG / 12.92 : pow(((vG + 0.055) / 1.055), 2.4);
            float lB = vB <= 0.04045 ? vB / 12.92 : pow(((vB + 0.055) / 1.055), 2.4);

            float y     = 0.2126 * lR + 0.7152 * lG + 0.0722 * lB;
            float lStar = y <= 216.0 / 24389.0 ? y * 24389.0 / 27.0 : pow(y, (1.0 / 3.0)) * 116.0 - 16.0;

            return lStar / 100.0;
        }

        inline
        int rgbToInt(int r, int g, int b)
        {
            int rgb = r;
            rgb     = (rgb << 8) + g;
            rgb     = (rgb << 8) + b;

            return rgb;
        }
    }
}

#endif