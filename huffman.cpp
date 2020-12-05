// huffman.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <vector>
#include <iostream>

#include "./include/cla_parse.hpp"
#include "./include/dir_func.hpp"
#include "./include/img_struct.hpp"
#include "./include/string_helper.hpp"


#define WINDOW_NAME "Huffman"

// CLA variable
std::string input_image;

const uint INTENSITY_VALUES = 256;

img_struct_t* og_image;
cv::Mat displayed_image;
cv::Point center;


// 'event loop' for keypresses
int
wait_key()
{
    char key_pressed = cv::waitKey(0) & 255;
    // 's' saves the current image
    if (cv::getWindowProperty(WINDOW_NAME, cv::WND_PROP_VISIBLE) < 1) {
        // this ends the program if window is closed
        return 0;
    }
    if (key_pressed == 's') {
        if (!displayed_image.empty()) {
            write_img_to_file(
                displayed_image,
                "./out",
                "output_" + input_image
            );
            cv::destroyAllWindows();
            return 0;
        }
    }
    // 'q' or  <escape> quits out
    if (key_pressed == 27 || key_pressed == 'q') {
        cv::destroyAllWindows();
        return 0;
    }
    return 1;
}


int
main(int argc, const char** argv)
{

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image
    );
    if (parse_result != 1) return parse_result;

    assert(input_image.length() > 0);

    // open image, grayscale = true
    og_image = open_image(input_image.c_str(), true);

    assert(og_image != NULL);

    // deep keep to displayed_image
    og_image->image.copyTo(displayed_image);

    // display the original image
    cv::imshow(WINDOW_NAME, displayed_image);

    int channels[] = { 0 };
    float range[] = { 0, 256 }; // the upper boundary is exclusive
    const float* histRange = { range };
    int histSize = 256;

    // calculate the histogram (counts)
    cv::Mat hist;
    cv::calcHist( &displayed_image, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, true, false );

    // normalize the histogram (probabilities)
    cv::Mat normal;
    cv::normalize( hist, normal, 0, displayed_image.rows, cv::NORM_MINMAX, -1, cv::Mat() );

    for ( int h = 0; h < histSize; h++ ) {
        float binVal = normal.at<float>(h);
        std::cout<<" "<<binVal;
    }

    // 'event loop' for keypresses
    while (wait_key());

    og_image->image.release();
    displayed_image.release();

    return 0;
}
