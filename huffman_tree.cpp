// huffman_tree.cpp : Convert color images to and from HSV
// Austin Hester CS542o dec 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0


#include "./include/huffman_tree.hpp"
#include "./include/huffman_tree_node.hpp"


// HuffmanTreeNode*
// build_internal(HuffmanTreeNode code1, HuffmanTreeNode code2)
// {
//     HuffmanTreeNode* root = NULL;
//     root = insert(root, code, 0);
//     int i;
//     for (i = 1; i < n; i++)
//         insert(root, wordlist[i], 0);
//     return root;
// }


HuffmanTreeNode*
initialize_node(HuffmanTreeNode* root, PixelProb pixel_prob, unsigned int depth)
{
    root = (HuffmanTreeNode*) malloc( sizeof(HuffmanTreeNode) );
    root->pixel_prob = { pixel_prob.symbol, pixel_prob.probability, { 0,0 } };
    root->depth = depth;
    root->left = NULL;
    root->right = NULL;
    return root;
}


HuffmanTreeNode*
insert(HuffmanTreeNode* root, PixelProb pixel_prob, unsigned int depth)
{
    if (root == NULL) {
        // initialize a new node
        root = initialize_node( root, pixel_prob, depth );
    }
    //  else if (len < root->length) {
    //     root->left = insert(root->left, word, ++level);
    // } else {
    //     root->right = insert(root->right, word, ++level);
    // }
    return root;
}


HuffmanTreeNode*
build_leaf(PixelProb pixel_prob)
{
    HuffmanTreeNode* root = NULL;
    root = insert( root, pixel_prob, 0 );
    return root;
}
