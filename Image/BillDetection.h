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
#include <opencv2/opencv.hpp>
#include <vector>

namespace Image {
    namespace BillDetection {
        cv::Mat highlightBill(cv::Mat in)
        {
            cv::Mat gray;
            cv::cvtColor(in, gray, cv::COLOR_BGR2GRAY);
            cv::GaussianBlur(gray, gray, cv::Size(3, 3), 0);

            cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Point(9, 9));

            cv::Mat dilated;
            cv::dilate(gray, dilated, kernel);

            cv::Mat edges;
            cv::Canny(dilated, edges, 84, 3);

            std::vector<cv::Vec4i> lines;
            lines.clear();

            cv::HoughLinesP(edges, lines, 1, CV_PI/180, 25);

            std::vector<cv::Vec4i>::iterator it = lines.begin();
            for(; it != lines.end(); ++it) {
                cv::Vec4i l = *it;
                cv::line(edges, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(255,0,0), 2, 8);
            }

            std::vector<std::vector<cv::Point>> contours;
            cv::findContours(edges, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_TC89_KCOS);

            std::vector<std::vector<cv::Point>> contoursCleaned;
            for (int i = 0; i < contours.size(); ++i) {
                if (cv::arcLength(contours[i], false) > 100) {
                    contoursCleaned.push_back(contours[i]);
                }
            }
            std::vector<std::vector<cv::Point>> contoursArea;
            for (int i = 0; i < contoursCleaned.size(); ++i) {
                if (cv::contourArea(contoursCleaned[i]) > 10000) {
                    contoursArea.push_back(contoursCleaned[i]);
                }
            }

            // Approximate polygon
            /* Question: we probably don't want a polygon all the time?! */
            // @todo bad implementation, focus on single square
            std::vector<std::vector<cv::Point> > contoursDraw (contoursArea.size());
            for (int i = 0; i < contoursArea.size(); ++i){
                cv::approxPolyDP(cv::Mat(contoursArea[i]), contoursDraw[i], 40, true);
            }

            cv::Mat mask = cv::Mat(in.size(), CV_8UC3, cv::Scalar(255, 255, 255));
            cv::drawContours(mask, contoursDraw, -1, cv::Scalar(0, 0, 0), cv::FILLED, 1);

            cv::Mat out = cv::Mat::zeros(in.size(), CV_8UC3);
            cv::bitwise_or(in, mask, out);

            return out;
        }
    }
}

#endif