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


const std::string WINDOW_NAME = "Huffman";
const int HIST_SIZE = 256;

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

// holds symbol and its probability
struct PixelProb {
    uint symbol;
    float probability;
};

// comparison function for struct PixelProb
bool
pixel_sorter(const PixelProb& a, const PixelProb& b)
{
    return a.probability < b.probability;
}


// compute probabilities
PixelProb*
compute_probabilities(cv::Mat histo)
{
    // get total pixels
    uint num_pixels = displayed_image.rows * displayed_image.cols;
    // get probabilities
    PixelProb* probabilities = (PixelProb*) malloc(sizeof(PixelProb) * HIST_SIZE);
    for ( uint h = 0; h < HIST_SIZE; h++ ) {
        float binVal = histo.at<float>(h);
        probabilities[h] = { h, binVal / num_pixels };
    }
    return probabilities;
}


// compute histogram
cv::Mat
compute_histogram(cv::Mat* img)
{
    int channels[] = { 0 };
    float range[] = { 0, (float) HIST_SIZE }; // the upper boundary is exclusive
    const float* histRange = { range };

    // calculate the histogram (counts)
    cv::Mat hist;
    cv::calcHist( img, 1, 0, cv::Mat(), hist, 1, &HIST_SIZE, &histRange, true, false );
    return hist;
}


int
main(int argc, const char** argv)
{

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image,
        NULL
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


    // compute histogram
    cv::Mat histo = compute_histogram( &displayed_image );

    // get probabilities
    PixelProb* probabilities = compute_probabilities( histo );

    // sort by probability
    std::sort(probabilities, probabilities + HIST_SIZE, &pixel_sorter);

    // display probs
    for ( uint h = 0; h < HIST_SIZE; h++ ) {
        float binVal = histo.at<float>(h);
        std::cout<<probabilities[h].symbol<<": "<<probabilities[h].probability<<std::endl;
    }

    cv::waitKey(500); // splash screen

    histo.release();
    displayed_image.release();
    og_image->image.release();

    delete probabilities;
    return 0;
}
