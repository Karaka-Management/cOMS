/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef IMAGE_DIFF_H
#define IMAGE_DIFF_H

#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "../Utils/MathUtils.h"

namespace Image
{
    namespace ImageUtils
    {
        cv::Mat find_diff (cv::Mat in1, cv::Mat in2)
        {
            cv::Mat diff;
            cv::absdiff(in1, in2, diff);

            int th = 10;
            cv::Mat mask(in1.size(), CV_8UC1);

            for (int i = 0; i < diff.rows; ++i) {
                for (int j = 0; j < diff.cols; ++j) {
                    cv::Vec3b px = diff.at<cv::Vec3b>(i, j);
                    int value    = px[0] + px[1] + px[2];

                    if (value > th) {
                        mask.at<unsigned char>(i, j) = 255;
                    }
                }
            }

            cv::Mat out;
            cv::bitwise_and(in2, in2, out, mask);

            return out;
        }
    }
}

#endif