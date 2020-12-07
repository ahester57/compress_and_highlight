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
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            img.at<uchar>(r, c) *= factor;
        }
    }
}

// dim an HSV image
void
dim_hsv_image(cv::Mat img, float factor)
{
    for (int r = 0; r < img.rows; r++) {
        for (int c = 0; c < img.cols; c++) {
            img.at<cv::Vec3b>(r, c)[2] *= factor;
        }
    }
}
