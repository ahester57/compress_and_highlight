// huffman_tree.hpp : Auxillary functions for huffman tree
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#ifndef HUFFMAN_TREE_H_
#define HUFFMAN_TREE_H_

#include "huffman_tree_node.hpp"

HuffmanTreeNode* build_leaf(PixelProb pixel_prob);
HuffmanTreeNode* combine_nodes(HuffmanTreeNode* node1, HuffmanTreeNode* node2);
HuffmanTreeNode** create_leaf_node_list(PixelProb* probabilities, int new_hist_size);
void print_huffman_table(HuffmanTreeNode* root);

#endif
