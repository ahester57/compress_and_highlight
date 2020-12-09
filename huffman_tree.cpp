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


// create a list of tree nodes, sorted by probability
HuffmanTreeNode**
create_tree_node_list(PixelProb* probabilities, int new_hist_size)
{
    HuffmanTreeNode** tree_nodes = (HuffmanTreeNode**) malloc( sizeof(HuffmanTreeNode*) * new_hist_size );
    // create leaf nodes from pixel probabilities
    for ( int i = 0; i < new_hist_size; i++ ) {
        tree_nodes[i] = build_leaf( probabilities[i] );
        // std::cout << tree_nodes[i]->pixel_prob.symbol << ": " << tree_nodes[i]->pixel_prob.probability << std::endl;

    }
    return tree_nodes;
}
