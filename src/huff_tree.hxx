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

// Encodes main function
void encode_huff(std::istream&, ipd::bostream&);

// Decodes main function
void decode_huff(ipd::bistream&, std::ostream&);

// Builds the tree using the priority queue
Node* build_tree(frequency_table_t const&);

// Builds the code word table of char(key) to the code of list-of-bool (value) using the helper function below
void build_code_word_table(Node *,
                           code_word_t,
                           code_word_table_t& cwt);

// Helper function that recursively creates the code for the char
void recursively_build_cwt(Node *,
                           bool,
                           code_word_t,
                           code_word_table_t& cwt);

// Serializes the tree to the huff file
void serialize_tree(Node const*, ipd::bostream&);

// Encodes each char into its respective bitcode
void encode_stream(code_word_table_t&,
                   std::istream&, ipd::bostream&);

// Deserializes the tree by reading the bits
Node* deserialize_tree(ipd::bistream&);

// Reads the bits and traverses the deserialized tree to give the necessary char
char decode_symbol(Node const*, ipd::bistream&, std::ostream& outfile);


