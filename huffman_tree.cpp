// huffman_tree.cpp : Auxillary functions for huffman tree
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <algorithm>
#include <assert.h>

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
    assert( node1 != NULL &&  node2 != NULL );
    HuffmanTreeNode* root = (HuffmanTreeNode*) malloc( sizeof(HuffmanTreeNode) );
    root->pixel_prob = { 999, node1->pixel_prob.probability + node2->pixel_prob.probability , { 0,0 } };
    root->depth = node2->depth+1;
    root->left = node1;
    root->right = node2;
    return root;
}


// create a list of tree nodes, sorted by probability
HuffmanTreeNode**
create_leaf_node_list(PixelProb* probabilities, unsigned int new_hist_size)
{
    assert( probabilities != NULL );
    HuffmanTreeNode** leaf_nodes = (HuffmanTreeNode**) malloc( sizeof(HuffmanTreeNode*) * new_hist_size );
    // create leaf nodes from pixel probabilities
    for ( int i = 0; i < new_hist_size; i++ ) {
        leaf_nodes[i] = build_leaf( probabilities[i] );
    }
    std::sort( leaf_nodes, leaf_nodes + new_hist_size, &huffman_heap_sorter );
    return leaf_nodes;
}


void
print_node(HuffmanTreeNode* node, std::string bit_string)
{
    assert( node != NULL );
    std::cout << node->to_string(bit_string) << std::endl;
}


// postorder (overloaded internal function)
void
print_huffman_table(HuffmanTreeNode* root, std::string bit_string, unsigned int str_length)
{
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL) {
        // root->pixel_prob.huffman_code = { bit_string, str_length };
        print_node( root, bit_string );
        return;
    }
    print_huffman_table( root->left, bit_string+"0", str_length+1 );
    print_huffman_table( root->right, bit_string+"1", str_length+1 );
}


// postorder
void
print_huffman_table(HuffmanTreeNode* root)
{
    assert( root != NULL );
    assert( root->left != NULL && root->right != NULL); // not ftrue huffman table if not

    std::cout << "\nHuffman codes: " << std::endl;
    print_huffman_table( root->left, "0", 1 );
    print_huffman_table( root->right, "1", 1 );
}
