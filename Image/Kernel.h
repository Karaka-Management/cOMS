/**
 * Jingga
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://jingga.app
 */
#ifndef IMAGE_KERNEL_H
#define IMAGE_KERNEL_H

#include <opencv2/opencv.hpp>
#include <stdio.h>

#include "../Utils/MathUtils.h"
#include "ImageUtils.h"

namespace Image::Kernel
{
    const float KERNEL_RIDGE_1[3][3] = {{0.0, -1.0, 0.0}, {-1.0, 4.0, -1.0}, {0.0, -1.0, 0.0}};

    const float KERNEL_RIDGE_2[3][3] = {{-1.0, -1.0, -1.0}, {-1.0, 8.0, -1.0}, {-1.0, -1.0, -1.0}};

    const float KERNEL_SHARPEN[3][3] = {{0.0, -1.0, 0.0}, {-1.0, 5.0, -1.0}, {0.0, -1.0, 0.0}};

    const float KERNEL_BOX_BLUR[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}, {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}};

    const float KERNEL_GAUSSUAN_BLUR_3[3][3] = {{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
                                                {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
                                                {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}};

    const float KERNEL_EMBOSS[3][3] = {{-2.0, -1.0, 0.0}, {-1.0, 1.0, 1.0}, {0.0, 1.0, 2.0}};

    const float KERNEL_UNSHARP_MASKING[5][5] = {
        {-1.0 / 256.0, -4.0 / 256.0, -6.0 / 256.0, -4.0 / 256.0, -1.0 / 256.0},
        {-4.0 / 256.0, -16.0 / 256.0, -24.0 / 256.0, -16.0 / 256.0, -4.0 / 256.0},
        {-6.0 / 256.0, -24.0 / 256.0, 476.0 / 256.0, -24.0 / 256.0, -6.0 / 256.0},
        {-4.0 / 256.0, -16.0 / 256.0, -24.0 / 256.0, -16.0 / 256.0, -4.0 / 256.0},
        {-1.0 / 256.0, -4.0 / 256.0, -6.0 / 256.0, -4.0 / 256.0, -1.0 / 256.0},
    };

    inline cv::Mat convolve(cv::Mat in, const float kernel[][3])
    {
        cv::Size dim = in.size();
        cv::Mat out(in.size(), in.type());

        cv::Mat mKernel(3, 3, CV_32F, (float *) kernel);
        cv::filter2D(in, out, -1, mKernel);

        return out;
    }
} // namespace Image::Kernel

#endif