#pragma once
#include "bit_io.hxx"
#include "common.hxx"
#include <iostream>
#include <map>
#include <queue>
// Citations:
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://www.youtube.com/watch?v=5knuxdsRVko

#define internal_node '\0'

using namespace std;
using namespace ipd;


class Node {
public:
    char data_;
    int freq_;
    vector<bool> code_;
    Node* left_;
    Node* right_;
    Node(char data, int freq){
        data_ = data;
        freq_ = freq;
        left_ = nullptr;
        right_ = nullptr;
    }
    // Comparison for frequency values
    struct node_cmp {
        bool operator()(Node* const& n1, Node* const& n2)
        {
            return n1->freq_ < n2->freq_;
        }
    };
};

class Tree {
public:
    // Frequency of every character is stored in here
    map<char, int> frequencyMap;
    // Build char (key) -> bit code (value) map based off input file
    map<char, vector<bool>> bitMap;
    pair<map<char, vector<bool>>::iterator,bool> bitPut;
    // Priority Queue
    priority_queue <Node *, vector<Node *>, Node::node_cmp> frequencyPQ;

    // Node list that will be used to make the bitMap.
    // This will probably just be taken out after we modify the current algorithm to
    // insert into the bitMap through tree traversal
    vector<Node*> nodesList;


    Tree(ifstream &infile, bostream &outfile){
        char c;
        pair<map<char,int>::iterator,bool> put;
        while (infile.read(&c, 1)) {
            put = frequencyMap.insert(pair<char, int>(c, 1));
            if (!put.second){
                frequencyMap[c] = frequencyMap[c] + 1;
            }
        }
        build_PQ();
        build_Tree();
        vector<bool> bools;
        get_Nodes(frequencyPQ.top(), bools);
        printList();
        serialize_tree(frequencyPQ.top(), outfile);
    }
    // Inserts into a PriorityQueue
    void build_PQ() {
        for (const auto &[k, v] : frequencyMap) {
            frequencyPQ.push(new Node(k, v));
        }
    }
    // Builds the tree based off the built PQ
    void build_Tree(){
        while (frequencyPQ.size() > 1) {
            // Remove first lowest
            Node* min1 = frequencyPQ.top();
            frequencyPQ.pop();

            // Remove second lowest
            Node* min2 = frequencyPQ.top();
            frequencyPQ.pop();

            // Merge into an internal node
            Node * new_internal_node = new Node(internal_node, min1->freq_ + min2->freq_);
            new_internal_node->left_ = min1;
            new_internal_node->right_ = min2;

            // Push the new internal node back into the frequencyPQ
            frequencyPQ.push(new_internal_node);
        }
    }

    // Prints the PQ
    void print_PQ(){
        //Prints out the PQ
        while (!frequencyPQ.empty()) {
            Node* p = frequencyPQ.top();
            frequencyPQ.pop();
            cout << p->data_ << " " << p->freq_ << "\n";
        }
    }

    // Serializes the tree to the huff file
    void serialize_tree(Node *root, bostream &outfile){
        if (!root) {
            return;
        }

        if (root->data_ != internal_node){
            outfile.write(false);
            outfile.write_bits(root->data_, 8);
        }
        else {
            outfile.write(true);
        }

        serialize_tree(root->left_, outfile);

        serialize_tree(root->right_, outfile);
    }

    // Creates the code for each char
    void get_Nodes(Node *root, vector<bool> bools) {
        if (!root){
            return;
        }

        if (root->data_ != internal_node) {
            root->code_ = bools;
            nodesList.push_back(root);
        }

        bools.push_back(false);
        get_Nodes(root->left_, bools);


        bools.push_back(true);
        get_Nodes(root->right_, bools);
    }

    void printList(){
        cout << "\nChar \t" << "Freq \t" << "Code \n\n";

        for (int i = 0; i < nodesList.size(); i++){
            cout << nodesList[i]->data_ << "\t " << nodesList[i]->freq_ << "\t ";
            // cout<< "Size: " << nodesList[i]->code_.size();
            for (int j = 0; j < nodesList[i]->code_.size(); j++) {
                if (nodesList[i]->code_[j]) {
                    cout << '1';
                }
                else {
                    cout << '0';
                }
            }
            cout << "\n";
        }
    }
};


