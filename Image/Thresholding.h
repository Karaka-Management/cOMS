/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef IMAGE_THRESHOLDING_H
#define IMAGE_THRESHOLDING_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "ImageUtils.h"
#include "../Utils/MathUtils.h"

namespace Image {
    class Thresholding {
        private:

        public:
            static
            cv::Mat integralThresholding(cv::Mat in)
            {
                cv::Size dim = in.size();
                cv::Mat out(in.size(), in.type());

                float intImg[dim.width][dim.height];
                float sum;

                cv::Vec3b bgr;
                for (int i = 0; i < dim.width; ++i) {
                    sum = 0.0;

                    for (int j = 0; j < dim.height; ++j) {
                        bgr = in.at<cv::Vec3b>(j, i);
                        sum += Image::ImageUtils::lightnessFromRgb(bgr[2], bgr[1], bgr[0]);

                        intImg[i][j] = i == 0 ? sum : intImg[i - 1][j] + sum;
                    }
                }

                int s = dim.width / 96.0;
                int t = 30;

                int x1, x2;
                int y1, y2;
                int count;
                float brightness;
                int color;

                for (int i = 0; i < dim.width; ++i) {
                    for (int j = 0; j < dim.height; ++j) {
                        x1 = max(1, i - s / 2.0);
                        x2 = min(i + s / 2.0, dim.width - 1);

                        y1 = max(1, j - s / 2.0);
                        y2 = min(j + s / 2.0, dim.height - 1);

                        count = (x2 - x1) * (y2 - y1);
                        sum   = intImg[x2][y2] - intImg[x2][y1 - 1] - intImg[x1 - 1][y2] + intImg[x1 - 1][y1 - 1];

                        bgr = in.at<cv::Vec3b>(j, i);
                        brightness = (float) Image::ImageUtils::lightnessFromRgb(bgr[2], bgr[1], bgr[0]);

                        color = brightness * count <= (sum * (100.0 - t) / 100.0) && brightness < 0.9 ? 0 : 255;

                        out.at<cv::Vec3b>(j, i)[0] = color;
                        out.at<cv::Vec3b>(j, i)[1] = color;
                        out.at<cv::Vec3b>(j, i)[2] = color;
                    }
                }

                return out;
            }
    };
}

#endif