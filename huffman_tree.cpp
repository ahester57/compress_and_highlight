// huffman_tree.cpp : Auxillary functions for huffman tree
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0


#include "./include/huffman_tree.hpp"
#include "./include/huffman_tree_node.hpp"


// build an original leaf node given a pixel probability
HuffmanTreeNode*
build_leaf(PixelProb pixel_prob)
{
    HuffmanTreeNode* root = (HuffmanTreeNode*) malloc( sizeof(HuffmanTreeNode) );
    root->pixel_prob = { pixel_prob.symbol, pixel_prob.probability, { 0,0 } };
    root->depth = 0;
    root->left = NULL;
    root->right = NULL;
    return root;
}


// combine two nodes (usually of the lowest probability)
HuffmanTreeNode*
combine_nodes(HuffmanTreeNode* node1, HuffmanTreeNode* node2)
{
    HuffmanTreeNode* root = (HuffmanTreeNode*) malloc( sizeof(HuffmanTreeNode) );
    root->pixel_prob = { 999, node1->pixel_prob.probability + node2->pixel_prob.probability , { 0,0 } };
    root->depth = node1->depth;
    root->left = node1;
    root->right = node2;
    return root;
}
