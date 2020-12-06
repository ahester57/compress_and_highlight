// hsv_convert.cpp : Convert color images to and from HSV
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "./include/hsv_convert.hpp"


// convert a BGR image to HSV
void
bgr_to_hsv(cv::Mat bgr, cv::Mat hsv)
{
    assert(bgr.channels() == 3);
}

// convert an HSV image to BGR
void
hsv_to_bgr(cv::Mat hsv, cv::Mat bgr)
{
    assert(hsv.channels() == 3);
}
