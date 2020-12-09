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
#include "./include/huffman_tree.hpp"
#include "./include/huffman_tree_node.hpp"
#include "./include/img_struct.hpp"
#include "./include/string_helper.hpp"


const std::string WINDOW_NAME = "Huffman";
const int HIST_SIZE = 256;


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


// compute probabilities
PixelProb*
compute_probabilities(cv::Mat histo, uint num_pixels)
{
    // get probabilities
    PixelProb* probabilities = (PixelProb*) malloc(sizeof(PixelProb) * HIST_SIZE);
    for ( uint i = 0; i < HIST_SIZE; i++ ) {
        probabilities[i] = { i,  histo.at<float>(i) / num_pixels };
    }
    return probabilities;
}


// sort by probability, removing 0 probability pixels
// returning new size
uint
filter_zero_probabilities(PixelProb** probabilities, uint hist_size)
{
    // count zeros
    uint num_zeros = 0;
    for ( uint i = 0; i < hist_size; i++ ) {
        if ( (*probabilities)[i].probability == 0 ) num_zeros++;
    }
    // allocate enough memory for nonzero symbols
    PixelProb* filtered_probabilities = (PixelProb*) malloc( sizeof(PixelProb) * (hist_size-num_zeros) );
    uint fp_index = 0;
    for ( uint i = 0; i < hist_size; i++ ) {
        if ( (*probabilities)[i].probability == 0 ) continue;
        // deep copy non zero PixelProbs
        filtered_probabilities[fp_index++] = { (*probabilities)[i].symbol, (*probabilities)[i].probability };
    }
    // delete probabilities and point it to the new list
    delete *probabilities;
    *probabilities = filtered_probabilities;
    // return size of new list
    return HIST_SIZE - num_zeros;
}


void
process_huffman(cv::Mat img)
{
    // compute histogram
    cv::Mat histo = compute_histogram( &img );

    // get probabilities
    PixelProb* probabilities = compute_probabilities( histo, img.rows * img.cols );

    // cleanup
    histo.release();

    assert( sizeof(probabilities) > 0 );

    // sort by probability
    uint new_hist_size = filter_zero_probabilities( &probabilities, HIST_SIZE );
    std::cout << "Number of non-zero probability symbols: " << new_hist_size << std::endl;

    // create a list of tree nodes, sorted by probability
    HuffmanTreeNode** leaf_nodes = create_tree_node_list( probabilities, new_hist_size );

    delete probabilities; // cleanup

    // sort the tree nodes by probability
    // pixel sorter in huffman_tree_node.hpp
    std::sort( leaf_nodes, leaf_nodes + new_hist_size, &huffman_heap_sorter );

    for ( uint i = 0; i < new_hist_size; i++ ) {
        std::cout << leaf_nodes[i]->pixel_prob.symbol << ": " << leaf_nodes[i]->pixel_prob.probability << std::endl;
    }

    // cleanup
    for ( uint i = 0; i < new_hist_size; i++ ) {
        delete leaf_nodes[i];
    }
    delete leaf_nodes;
}


int
main(int argc, const char** argv)
{
    // CLA variable
    std::string input_image;

    // parse and save command line args
    int parse_result = parse_arguments(
        argc, argv,
        &input_image,
        NULL
    );
    if ( parse_result != 1 ) return parse_result;

    assert( input_image.length() > 0 );

    img_struct_t* og_image;

    // open image, grayscale = true
    og_image = open_image( input_image.c_str(), true );

    assert( og_image != NULL );

    // display the original image
    cv::imshow( WINDOW_NAME, og_image->image );

    process_huffman(og_image->image);

    cv::waitKey(999); // splash screen

    // cleanup
    og_image->image.release();
    return 0;
}
