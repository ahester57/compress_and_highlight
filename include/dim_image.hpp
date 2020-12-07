// dim_image.hpp : Dim images
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef DIM_IMAGE_H
#define DIM_IMAGE_H

#include <opencv2/core/core.hpp>

void dim_grayscale_image(cv::Mat img, float factor);
void dim_hsv_image(cv::Mat img, float factor);

#endif
