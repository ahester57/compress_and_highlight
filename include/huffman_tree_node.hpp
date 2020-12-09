// huffman_tree_node.hpp : Convert color images to and from HSV
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef HUFFMAN_TREE_NODE_H_
#define HUFFMAN_TREE_NODE_H_

#include <bitset>
#include <iostream>


// holds code and length of code
struct HuffmanCode {
    unsigned int length;
    unsigned int code;

    std::string to_string() {
        return std::bitset<64>( code ).to_string();
    }
};


// holds symbol and its probability
struct PixelProb {
    unsigned int symbol;
    float probability;
    HuffmanCode huffman_code;
};


// comparison function for struct PixelProb
static bool
pixel_sorter(const PixelProb& a, const PixelProb& b)
{
    return a.probability < b.probability;
};


struct HuffmanTreeNode
{
    PixelProb pixel_prob;
    unsigned int depth;
    PixelProb* left;
    PixelProb* right;
};


// comparison function for struct HuffmanTreeNode
static bool
huffman_heap_sorter(const HuffmanTreeNode& a, const HuffmanTreeNode& b)
{
    return a.pixel_prob.probability < b.pixel_prob.probability;
};


#endif
