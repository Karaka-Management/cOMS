/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef IMAGE_KERNEL_H
#define IMAGE_KERNEL_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "ImageUtils.h"
#include "../Utils/MathUtils.h"

namespace Image {
    const float KERNEL_RIDGE_1[3][3] = {
        {0.0, -1.0, 0.0},
        {-1.0, 4.0, -1.0},
        {0.0, -1.0, 0.0}
    };

    const float KERNEL_RIDGE_2[3][3] = {
        {-1.0, -1.0, -1.0},
        {-1.0, 8.0, -1.0},
        {-1.0, -1.0, -1.0}
    };

    const float KERNEL_SHARPEN[3][3] = {
        {0.0, -1.0, 0.0},
        {-1.0, 5.0, -1.0},
        {0.0, -1.0, 0.0}
    };

    const float KERNEL_BOX_BLUR[3][3] = {
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
        {1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
    };

    const float KERNEL_GAUSSUAN_BLUR_3[3][3] = {
        {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
        {2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
        {1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
    };

    const float KERNEL_EMBOSS[3][3] = {
        {-2.0, -1.0, 0.0},
        {-1.0, 1.0, 1.0},
        {0.0, 1.0, 2.0}
    };

    const float KERNEL_UNSHARP_MASKING[5][5] = {
        {-1.0 / 256.0,  -4.0 / 256.0, -6.0 / 256.0, -4.0 / 256.0, -1.0 / 256.0},
        {-4.0 / 256.0,  -16.0 / 256.0, -24.0 / 256.0, -16.0 / 256.0, -4.0 / 256.0},
        {-6.0 / 256.0,  -24.0 / 256.0, 476.0 / 256.0, -24.0 / 256.0, -6.0 / 256.0},
        {-4.0 / 256.0,  -16.0 / 256.0, -24.0 / 256.0, -16.0 / 256.0, -4.0 / 256.0},
        {-1.0 / 256.0,  -4.0 / 256.0, -6.0 / 256.0, -4.0 / 256.0, -1.0 / 256.0},
    };

    class Kernel {
        private:

        public:

            /**
             * @see https://en.wikipedia.org/wiki/Kernel_(image_processing)
             * @see https://towardsdatascience.com/image-processing-with-python-blurring-and-sharpening-for-beginners-3bcebec0583a
             * @see https://web.eecs.umich.edu/~jjcorso/t/598F14/files/lecture_0924_filtering.pdf
             */
            static
            cv::Mat convolve(cv::Mat in, const float kernel[][3])
            {
                cv::Size dim = in.size();
                cv::Mat out(in.size(), in.type());

                int kernel_dimension = 3;

                int kWidthRadius  = (unsigned int) kernel_dimension >> 1;
                int kHeightRadius = (unsigned int) kernel_dimension >> 1;

                int newPixel;
                cv::Vec3b p;

                // @todo: implement @see https://rosettacode.org/wiki/Image_convolution
                // @todo: not working yet
                for (int i = dim.width - 1; i >= 0; --i) {
                    for (int j = dim.height - 1; j >= 0; --j) {
                        newPixel = 0;

                        for (int ki = kernel_dimension - 1; ki >= 0; --ki) {
                            for (int kj = kernel_dimension - 1; kj >= 0; --kj) {
                                p = in.at<cv::Vec3b>(
                                    oms_min(oms_max(j + kj - kHeightRadius, 0), dim.height - 1),
                                    oms_min(oms_max(i + ki - kWidthRadius, 0), dim.width - 1)
                                );

                                newPixel += kernel[ki][kj] * Image::ImageUtils::rgbToInt(p[2], p[1], p[0]);
                            }
                        }

                        // bgr vs rgb
                        out.at<cv::Vec3b>(j, i)[0] = newPixel & 255;
                        out.at<cv::Vec3b>(j, i)[1] = (newPixel >> 8) & 255;
                        out.at<cv::Vec3b>(j, i)[2] = (newPixel >> 16) & 255;
                    }
                }

                return out;
            }
    };
}

#endif