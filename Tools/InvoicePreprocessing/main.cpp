/**
 * Karaka
 *
 * @package   App
 * @copyright Dennis Eichhorn
 * @license   OMS License 1.0
 * @version   1.0.0
 * @link      https://karaka.app
 */
#include <stdio.h>
#include <opencv2/opencv.hpp>

#include "../../Image/Skew.h"
#include "../../Image/Thresholding.h"

int main(int argc, char** argv)
{
    if (argc != 3) {
        printf("A input image and a output image is required\n");

        return -1;
    }

    cv::Mat in;
    in = cv::imread(argv[1], cv::IMREAD_UNCHANGED);
    if (!in.data) {
        printf("Couldn't read image.\n");

        return -1;
    }

    cv::Mat out = Image::Thresholding::integralThresholding(in);
    out = Image::Skew::deskewHoughLines(out);

    cv::imwrite(argv[2], out);

    /*
    cv::imshow("in", in);
    cv::imshow("out", out);
    cv::waitKey(0);
    */

    return 0;
}