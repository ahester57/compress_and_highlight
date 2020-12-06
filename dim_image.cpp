// dim_image.cpp : Dim images
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "./include/dim_image.hpp"


// dim a grayscale image
void
dim_grayscale_image(cv::Mat img, float factor)
{
    // convert image to float
    cv::Mat tmp;
    img.convertTo(tmp, CV_32F, 1/255.0);

    for (int r = 0; r < tmp.rows; r++) {
        for (int c = 0; c < tmp.cols; c++) {
            tmp.at<float>(r, c) = tmp.at<float>(r, c) * factor;
        }
    }

    // convert back to CV_8UC3
    tmp.convertTo(img, CV_8U, 255.0);
}
