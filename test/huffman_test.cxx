#include <catch.h>
#include "huff_tree.hxx"
#include <iostream>
#include <sstream>

using namespace std;
using namespace ipd;

static void round_trip(string const& msg)
{
    istringstream is(msg);
    bostringstream boss;
    encode_huff(is, boss);

    bistringstream biss(boss.data());
    ostringstream os;
    decode_huff(biss, os);

    CHECK(os.str() == msg);
}

TEST_CASE("Build Tree")
{
    // Frequency Map
    frequency_table_t frequencyMap;
    frequencyMap['a'] = 5;
    frequencyMap['b'] = 10;
    frequencyMap['c'] = 20;
    frequencyMap['d'] = 40;
    frequencyMap['e'] = 80;
    frequencyMap['f'] = 160;

    // Result from build_tree
    Node* result = build_tree(frequencyMap);

    // Leaf Nodes
    Node* a = new Node('a', 5);
    Node* b = new Node('b', 10);
    Node* c = new Node('c', 20);
    Node* d = new Node('d', 40);
    Node* e = new Node('e', 80);
    Node* f = new Node('f', 160);

    // Internal Nodes
    Node* internal_node1 = new Node(a, b);
    Node* internal_node2 = new Node(internal_node1, c);
    Node* internal_node3 = new Node(internal_node2, d);
    Node* internal_node4 = new Node(internal_node3, e);
    Node* internal_node5 = new Node(internal_node4, f);
    // Tests leaf node is 'f'
    CHECK(result->right_->data_ ==
    internal_node5->right_->data_);

    // Tests leaf node is 'e'
    CHECK(result->left_->right_->data_ ==
    internal_node5->left_->right_->data_);

    // Tests leaf node is 'd'
    CHECK(result->left_->left_->right_->data_ ==
    internal_node5->left_->left_->right_->data_);

    // Tests leaf node is 'c'
    CHECK(result->left_->left_->left_->right_->data_ ==
    internal_node5->left_->left_->left_->right_->data_);

    // Tests leaf node is 'b'
    CHECK(result->left_->left_->left_->left_->right_->data_ ==
    internal_node5->left_->left_->left_->left_->right_->data_);

    // Tests leaf node is 'a'
    CHECK(result->left_->left_->left_->left_->left_->data_ ==
    internal_node5->left_->left_->left_->left_->left_->data_);
}

TEST_CASE("Build_Code_Word_Table")
{
    // Frequency Map
    frequency_table_t frequencyMap;
    frequencyMap['a'] = 5;
    frequencyMap['b'] = 10;
    frequencyMap['c'] = 20;
    frequencyMap['d'] = 40;
    frequencyMap['e'] = 80;
    frequencyMap['f'] = 160;

    // Result from build_tree
    Node* huffman_tree = build_tree(frequencyMap);

    // Stores new code word table
    code_word_table_t cwt;
    build_code_word_table(huffman_tree, {}, cwt);


    vector<bool> a_bools = {false, false, false, false, false};
    vector<bool> b_bools = {false, false, false, false, true};
    vector<bool> c_bools = {false, false, false, true};
    vector<bool> d_bools = {false, false, true};
    vector<bool> e_bools = {false, true};
    vector<bool> f_bools = {true};
    CHECK(cwt['a'] == a_bools);
    CHECK(cwt['b'] == b_bools);
    CHECK(cwt['c'] == c_bools);
    CHECK(cwt['d'] == d_bools);
    CHECK(cwt['e'] == e_bools);
    CHECK(cwt['f'] == f_bools);
}

TEST_CASE("Serialize and Deserialize Tree")
{
    // This is the frequency inputted into huff_test.txt
//    frequencyMap['a'] = 5;
//    frequencyMap['b'] = 10;
//    frequencyMap['c'] = 20;
//    frequencyMap['d'] = 40;
//    frequencyMap['e'] = 80;
//    frequencyMap['f'] = 160;
    istringstream is("aaaaabbbbbbbbbbccccccccccccccccccccddddddddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
    bostringstream boss;
    char c;
    frequency_table_t frequencyMap;
    size_t len = 0;

    while (is.read(&c, 1)) {
        ++frequencyMap[c];
        ++len;
    }

    // Encoding Padding
    boss.write_bits(len, CHAR_BIT * sizeof len);

    // Result from build_tree
    Node* result = build_tree(frequencyMap);

    // Serialize tree
    serialize_tree(result, boss);

    bistringstream boss_in(boss.data());

    // Decoding Padding
    boss_in.read_bits(len, CHAR_BIT * sizeof len);

    // Deserialize tree
    auto deserialize_result = deserialize_tree(boss_in);

    // Tests leaf node is 'f'
    CHECK(result->right_->data_ ==
          deserialize_result->right_->data_);

    // Tests leaf node is 'e'
    CHECK(result->left_->right_->data_ ==
          deserialize_result->left_->right_->data_);

    // Tests leaf node is 'd'
    CHECK(result->left_->left_->right_->data_ ==
          deserialize_result->left_->left_->right_->data_);

    // Tests leaf node is 'c'
    CHECK(result->left_->left_->left_->right_->data_ ==
          deserialize_result->left_->left_->left_->right_->data_);

    // Tests leaf node is 'b'
    CHECK(result->left_->left_->left_->left_->right_->data_ ==
          deserialize_result->left_->left_->left_->left_->right_->data_);

    // Tests leaf node is 'a'
    CHECK(result->left_->left_->left_->left_->left_->data_ ==
          deserialize_result->left_->left_->left_->left_->left_->data_);
}

TEST_CASE("Round trip A")
{
    round_trip("A");
}

TEST_CASE("Round trip hello")
{
    round_trip("hello");
}

TEST_CASE("Round trip Hello world")
{
    round_trip("Hello, world!");
}