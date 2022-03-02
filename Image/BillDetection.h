/**
 * Karaka
 *
 * @package   Image
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#ifndef IMAGE_BILL_DETECTION_H
#define IMAGE_BILL_DETECTION_H

#include <stdio.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <vector>

#include "../Utils/MathUtils.h"

namespace Image {
    class BillDetection {
        private:
            static
            double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
            {
                double dx1 = pt1.x - pt0.x;
                double dy1 = pt1.y - pt0.y;
                double dx2 = pt2.x - pt0.x;
                double dy2 = pt2.y - pt0.y;

                return (dx1 * dx2 + dy1 * dy2) / sqrt((dx1 * dx1 + dy1 * dy1) * (dx2 * dx2 + dy2 * dy2) + 1e-10);
            }

        public:
            static
            cv::Mat highlightBill(cv::Mat in)
            {
                const int MORPH = 9;
                const int CANNY = 84;
                const int HOUGH = 25;

                cv::Mat grey;
                cv::cvtColor(in, grey, cv::COLOR_BGR2GRAY);

                cv::Size ksize = cv::Size(3, 3);
                cv::GaussianBlur(grey, grey, ksize, 0);

                ksize = cv::Size(MORPH, MORPH);
                cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, ksize);

                cv::Mat dilated;
                cv::dilate(grey, dilated, kernel);

                cv::Mat edges;
                cv::Canny(dilated, edges, 0, CANNY, 3);

                std::vector<cv::Vec4i> lines;
                cv::HoughLinesP(edges, lines, 1.0, M_PI / 180, HOUGH);

                for (int i = 0; i < lines.size(); ++i) {
                    cv::Point p1 = cv::Point(lines[i][0], lines[i][1]);
                    cv::Point p2 = cv::Point(lines[i][2], lines[i][3]);

                    cv::Scalar rgb = cv::Scalar(255, 0, 0);

                    cv::line(edges, p1, p2, rgb, 2, 8);
                }

                std::vector<std::vector<cv::Point>> contours;
                cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

                // filter lines
                std::vector<std::vector<cv::Point>>::iterator it;
                int i = 0;
                for (it = contours.begin(); it != contours.end(); ++it, ++i) {
                    if (cv::arcLength(*it, false) < 100) {
                        contours.erase(contours.begin() + i);
                    }
                }

                for (it = contours.begin(); it != contours.end(); ++it, ++i) {
                    if (cv::contourArea(*it) < 10000) {
                        contours.erase(contours.begin() + i);
                    }
                }

                // create polygons
                std::vector<std::vector<cv::Point>> rect;
                for (int i = 0; contours.size(); ++i) {
                    cv::approxPolyDP(contours[i], rect, 40, true);
                }

                cv::Mat out;

                cv::imshow("in", in);
                cv::imshow("out", dilated);
                cv::imshow("out", edges);
                cv::imshow("out", out);
                cv::waitKey(0);

                return out;
            }

            static
            std::vector<std::vector<cv::Point>> findSquares(cv::Mat in)
            {
                cv::Mat blurred(in);
                cv::medianBlur(in, blurred, 9);

                cv::Mat gray0(blurred.size(), CV_8U), gray;
                std::vector<std::vector<cv::Point>> contours;

                std::vector<std::vector<cv::Point>> squares;

                // find squares in every color plane of the image
                for (int c = 0; c < 3; c++) {
                    int ch[] = {c, 0};
                    cv::mixChannels(&blurred, 1, &gray0, 1, ch, 1);

                    // try several threshold levels
                    const int threshold_level = 2;
                    for (int l = 0; l < threshold_level; ++l) {
                        // Canny helps to catch squares with gradient shading
                        if (l == 0) {
                            cv::Canny(gray0, gray, 10, 20, 3);

                            // Dilate helps to remove potential holes between edge segments
                            cv::dilate(gray, gray, cv::Mat(), cv::Point(-1, -1));
                        } else {
                            gray = gray0 >= (l + 1) * 255 / threshold_level;
                        }

                        cv::findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

                        // Test contours
                        std::vector<cv::Point> approx;
                        for (size_t i = 0; i < contours.size(); ++i) {
                            cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.02, true);

                            // contour orientation
                            if (approx.size() == 4 &&
                                fabs(cv::contourArea(cv::Mat(approx))) > 1 &&
                                cv::isContourConvex(cv::Mat(approx))
                            ) {
                                double maxCosine = 0;

                                for (int j = 2; j < 5; ++j) {
                                    double cosine = fabs(angle(approx[j % 4], approx[j - 2], approx[j - 1]));
                                    maxCosine     = max(maxCosine, cosine);
                                }

                                if (maxCosine < 0.3) {
                                    squares.push_back(approx);
                                }
                            }
                        }
                    }
                }

                return squares;
            }

            static
            cv::Mat debugSquares( std::vector<std::vector<cv::Point>> squares, cv::Mat in)
            {
                for (int i = 0; i <squares.size(); ++i) {
                    cv::drawContours(in, squares, i, cv::Scalar(255,0,0), 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());

                    cv::Rect rect = boundingRect(cv::Mat(squares[i]));
                    cv::rectangle(in, rect.tl(), rect.br(), cv::Scalar(0,255,0), 2, 8, 0);

                    cv::RotatedRect minRect = minAreaRect(cv::Mat(squares[i]));
                    cv::Point2f rect_points[4];
                    minRect.points(rect_points);

                    for ( int j = 0; j < 4; j++ ) {
                        cv::line(in, rect_points[j], rect_points[(j + 1) % 4], cv::Scalar(0,0,255), 1, 8);
                    }
                }

                return in;
            }
    };
}

#endif