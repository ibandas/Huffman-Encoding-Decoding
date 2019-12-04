#pragma once
#include "bit_io.hxx"
#include "common.hxx"
#include <iostream>
#include <map>
#include <queue>

// Citations:
// https://www.geeksforgeeks.org/huffman-coding-greedy-algo-3/
// https://www.youtube.com/watch?v=5knuxdsRVko

using namespace std;
using namespace ipd;

using frequency_table_t = std::map<char, size_t>;
using code_word_t       = std::vector<bool>;
using code_word_table_t = std::map<char, code_word_t>;

class Node {
public:
    char data_;
    int freq_;
    Node* left_;
    Node* right_;

    Node(char data, int freq)
            : data_(data)
            , freq_(freq)
            , left_(nullptr)
            , right_(nullptr)
    { }

    Node(Node* left, Node* right)
            : data_(0)
            , freq_(left->freq_ + right->freq_)
            , left_(left)
            , right_(right)
    { }

    bool is_leaf() const
    {
        return left_ == nullptr;
    }

    // Comparison for frequency values
    struct node_cmp {
        bool operator()(Node* const& n1, Node* const& n2)
        {
            return n1->freq_ > n2->freq_;
        }
    };
};

class HuffmanTree {
public:
    explicit HuffmanTree(frequency_table_t const&);
    explicit HuffmanTree(ipd::bistream&);

    char decode_symbol(ipd::bistream&) const;
    void serialize(ipd::bostream&) const;

    ~HuffmanTree();

private:
    Node* root_;
};

void encode(std::istream&, ipd::bostream&);

void decode(ipd::bistream&, std::ostream&);

Node* build_tree(frequency_table_t const&);

void build_code_word_table(Node const*,
                           code_word_t const&,
                           code_word_table_t& cwt);

// Serializes the tree to the huff file
void serialize_tree(Node const*, ipd::bostream&);

void encode_stream(code_word_table_t const&,
                   std::istream&, ipd::bostream&);

Node* deserialize_tree(ipd::bistream&);

char decode_symbol(Node const*, ipd::bistream&);

class Tree {
public:
    // Frequency of every character is stored in here
    frequency_table_t frequencyMap;

    // Build char (key) -> bit code (value) map based off input file
    code_word_table_t bitMap;

    // Priority Queue
    priority_queue <Node *, vector<Node *>, Node::node_cmp> frequencyPQ;

    // Inserts into a PriorityQueue
    void build_PQ() {
        for (const auto &[k, v] : frequencyMap) {
            frequencyPQ.push(new Node(k, v));
        }
    }
    // Builds the tree based off the built PQ
    void build_Tree();

    // Prints the PQ
    void print_PQ(){
        //Prints out the PQ
        while (!frequencyPQ.empty()) {
            Node* p = frequencyPQ.top();
            frequencyPQ.pop();
            cout << p->data_ << " " << p->freq_ << "\n";
        }
    }

    // Creates the code for each char
    void get_Nodes(Node *root, bool val, code_word_t bools) {
        if (!root){
            return;
        }
        bools.push_back(val);
        if (root->is_leaf()) {
//            root->code_ = bools;
//            nodesList.push_back(root);
        }
        
        get_Nodes(root->left_, false, bools);

        get_Nodes(root->right_, true, bools);
    }

//    void printList(){
//        cout << "\nChar \t" << "Freq \t" << "Code \n\n";
//
//        for (int i = 0; i < nodesList.size(); i++){
//            cout << nodesList[i]->data_ << "\t " << nodesList[i]->freq_ << "\t ";
//            // cout<< "Size: " << nodesList[i]->code_.size();
//            for (int j = 0; j < nodesList[i]->code_.size(); j++) {
//                if (nodesList[i]->code_[j]) {
//                    cout << '1';
//                }
//                else {
//                    cout << '0';
//                }
//            }
//            cout << "\n";
//        }
//    }
};


