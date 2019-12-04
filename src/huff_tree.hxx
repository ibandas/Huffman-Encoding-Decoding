#pragma once
#include "bit_io.hxx"
#include <iostream>
#include <map>
// Citations:
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://www.youtube.com/watch?v=5knuxdsRVko

using namespace std;


class Node {
public:
    char data_;
    int freq_;
    string code_;
    Node* left_;
    Node* right_;
    Node(char data, int freq){
        data_ = data;
        freq_ = freq;
        code_ = "";
        left_ = nullptr;
        right_ = nullptr;
    }
};








// This tree is constructed based off a map with all the ASCII character and frequency value pairs
//class HuffTree {
//public:
//    HuffTree();
//    // Constructs a tree based on given input
//    HuffTree(map<char, unsigned>);
//    // Inserts into the tree
//    void insert(char data, unsigned freq);
//    // This will sort based on frequency
//    map<char, unsigned> sort_map(map<char, unsigned>);
//    // This function will serialize the tree
//    void create_bit_code();
//private:
//    struct node_;
//    using link_ = unique_ptr<node_>;
//    struct node_ {
//        char data;
//        unsigned freq;
//        link_ left;
//        link_ right;
//    };
//    node_ root_;
//    size_t size_;
//};
//
//void HuffTree::create_bit_code() {
//
//}


