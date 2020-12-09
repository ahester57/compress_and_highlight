// huffman_tree_node.hpp : Data Structures and sort functions for Huffman Tree
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef HUFFMAN_TREE_NODE_H_
#define HUFFMAN_TREE_NODE_H_

#include <bitset>
#include <iostream>


// holds code and length of code (needed?)
struct HuffmanCode {
    unsigned int code;
    unsigned int length;

    std::bitset<32> to_bitset() {
        return std::bitset<32>( code );
    }
};


// holds symbol and its probability
struct PixelProb {
    unsigned int symbol;
    float probability;
    HuffmanCode huffman_code;
};


// The focal point of our Huffman Tree
struct HuffmanTreeNode
{
    PixelProb pixel_prob;
    unsigned int depth;
    HuffmanTreeNode* left;
    HuffmanTreeNode* right;

    std::string
    to_string(std::string tmp_bitstring_til_fixed) {
        return std::to_string(pixel_prob.symbol) + "\t: " +
                std::to_string(pixel_prob.probability) + "\t: " +
                tmp_bitstring_til_fixed;
    };
};


// comparison function for struct HuffmanTreeNode
static bool
huffman_heap_sorter(const HuffmanTreeNode* a, const HuffmanTreeNode* b)
{
    if ( a == NULL ) return false; // { NULLs get pushed to back }
    if ( b == NULL ) return true;  // {                          }
    return a->pixel_prob.probability > b->pixel_prob.probability;
};


#endif
