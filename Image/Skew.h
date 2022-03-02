/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef IMAGE_SKEW_H
#define IMAGE_SKEW_H

#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "../Utils/MathUtils.h"

namespace Image {
    class Skew {
        private:

        public:
            static
            cv::Mat deskewHoughLines(cv::Mat in, int maxDegree = 45)
            {
                cv::Size dim = in.size();

                cv::Mat inv;
                cv::cvtColor(in, inv, cv::COLOR_BGR2GRAY);
                cv::threshold(inv, inv, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);

                std::vector<cv::Vec4i> lines;
                cv::HoughLinesP(inv, lines, 1.0, M_PI / 180, 200, dim.width / 12, dim.width / 150);

                int imageOrientation = 0; // > 0 -> horizontal

                std::vector<float> tmpAngles;
                for (int i = 0; i < lines.size(); ++i) {
                    float angle = atan2(lines[i][3] - lines[i][1], lines[i][2] - lines[i][0]);
                    tmpAngles.push_back(angle);

                    imageOrientation += abs(angle) > M_PI / 4 ? 1 : -1;
                }

                std::vector<float> angles;
                for (int i = 0; i < tmpAngles.size(); ++i) {
                    if (imageOrientation > 0) {
                        if (deg2rad(90 - maxDegree) < abs(tmpAngles[i]) < deg2rad(90 + maxDegree)) {
                            angles.push_back(tmpAngles[i]);
                        }
                    } else {
                        if (abs(tmpAngles[i]) < deg2rad(maxDegree)) {
                            angles.push_back(tmpAngles[i]);
                        }
                    }
                }

                if (angles.size() < 5) {
                    return in;
                }

                float median = 0.0;
                for (int i = 0; i < angles.size(); ++i) {
                    median += angles[i];
                }

                float angleDeg = rad2deg(median / angles.size());

                cv::Mat orientFix;
                if (imageOrientation > 0) {
                    if (angleDeg < 0) {
                        cv::rotate(in, orientFix, cv::ROTATE_90_CLOCKWISE);
                        angleDeg += 90.0;
                    } else if (angleDeg > 0) {
                        cv::rotate(in, orientFix, cv::ROTATE_90_COUNTERCLOCKWISE);
                        angleDeg -= 90.0;
                    } else {
                        orientFix = in;
                    }
                } else {
                    orientFix = in;
                }

                cv::Mat rot = cv::getRotationMatrix2D(cv::Point2f(dim.width / 2, dim.height / 2), angleDeg, 1.0);

                cv::Mat out;
                cv::warpAffine(orientFix, out, rot, dim, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

                return out;
            }
    };
}

#endif