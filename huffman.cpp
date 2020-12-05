// huffman.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <bitset>
#include <iostream>
#include <vector>

#include "./include/cla_parse.hpp"
#include "./include/dir_func.hpp"
#include "./include/img_struct.hpp"
#include "./include/string_helper.hpp"


#define WINDOW_NAME "Huffman"

// CLA variable
std::string input_image;

img_struct_t* og_image;
cv::Mat displayed_image;

// holds code and length of code
struct HuffmanCode {
    uint length;
    uint code;

    std::string to_string() {
        return std::bitset<64>( code ).to_string();
    }
};

struct PixelProb {
    uint symbol;
    float probability;
};


void
get_histogram(cv::Mat img, cv::Mat* dst)
{
    int channels[] = { 0 };
    float range[] = { 0, 256 }; // the upper boundary is exclusive
    const float* histRange = { range };
    int histSize = 256;

    // calculate the histogram (counts)
    cv::calcHist( &img, 1, 0, cv::Mat(), *dst, 1, &histSize, &histRange, true, false );
}


int
main(int argc, const char** argv)
{

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image
    );
    if ( parse_result != 1 ) return parse_result;

    assert(input_image.length() > 0);

    // open image, grayscale = true
    og_image = open_image( input_image.c_str(), true );

    assert(og_image != NULL);

    // deep keep to displayed_image
    og_image->image.copyTo(displayed_image);

    // display the original image
    cv::imshow(WINDOW_NAME, displayed_image);

    int histSize = 256;
    cv::Mat histo;
    // normalize the histogram (probabilities)
    get_histogram( displayed_image, &histo );

    //
    uint num_pixels = displayed_image.rows * displayed_image.cols;

    PixelProb probabilities[histSize];
    for ( uint h = 0; h < histSize; h++ ) {
        float binVal = histo.at<float>(h);
        probabilities[h] = { h, binVal / num_pixels };
    }

    for ( uint h = 0; h < histSize; h++ ) {
        float binVal = histo.at<float>(h);
        std::cout<<probabilities[h].symbol<<": "<<probabilities[h].probability<<std::endl;
    }

    // cv::Mat sorted = cv::Mat::zeros( normal.size(), normal.type() );
    // cv::sort( normal, sorted, cv::SORT_EVERY_COLUMN );

    og_image->image.release();
    displayed_image.release();

    return 0;
}
