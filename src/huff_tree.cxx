#include "huff_tree.hxx"
#include <map>
#include <queue>

using namespace std;
using namespace ipd;

void encode_huff(std::istream &infile, ipd::bostream &outfile)
{
    size_t len = 0;
    char c;

    frequency_table_t frequencyMap;

    while (infile.read(&c, 1)) {
        ++frequencyMap[c];
        ++len;
    }

    auto huffman_tree = build_tree(frequencyMap);

    code_word_table_t cwt;
    build_code_word_table(huffman_tree, false, {}, cwt);
    outfile.write_bits(len, CHAR_BIT * sizeof len);
    serialize_tree(huffman_tree, outfile);
//    encode_stream(cwt, infile, outfile);
}

Node* build_tree(frequency_table_t const& frequencyMap)
{
    // Builds the priority queue
    priority_queue <Node*, vector<Node*>, Node::node_cmp> frequencyPQ;
    for (const auto &[k, v] : frequencyMap) {
        frequencyPQ.push(new Node(k, v));
    }
    while (frequencyPQ.size() > 1) {
        // Remove first lowest
        Node* min1 = frequencyPQ.top();
        frequencyPQ.pop();

        // Remove second lowest
        Node* min2 = frequencyPQ.top();
        frequencyPQ.pop();

        // Push the new internal node back into the frequencyPQ
        frequencyPQ.push(new Node(min1, min2));
    }
    return frequencyPQ.top();

}

// Creates the code for each char
void build_code_word_table(Node *root, bool val, code_word_t bools, code_word_table_t& cwt) {
    if (!root){
        return;
    }
    bools.push_back(val);
    if (root->is_leaf()) {
        cwt[root->data_] = bools;
    }

    build_code_word_table(root->left_, false, bools, cwt);

    build_code_word_table(root->right_, true, bools, cwt);
}


void serialize_tree(Node const* node, bostream &outfile){
    if (node->is_leaf()){
        cout << "Leaf";
        outfile.write(false);
        outfile.write_bits(node->data_, 8);
    } else {
        cout << "Internal";
        outfile.write(true);
        serialize_tree(node->left_, outfile);
        serialize_tree(node->right_, outfile);
    }
}

void encode_stream(code_word_table_t& cwt, std::istream& infile, ipd::bostream& out){
    // Converts each character from the infile to it's respective bit code into the huff file
    char x;
    infile.clear();
    infile.seekg (0, ios::beg);
    while (infile.read(&x, 1)) {
        for (int i = 0; i < cwt[x].size(); i++) {
            out.write(cwt[x][i]);
        }
    }
}

void decode_huff(ipd::bistream& infile, std::ostream& outfile)
{
    size_t len;

    infile.read_bits(len, CHAR_BIT * sizeof len);
    auto tree = deserialize_tree(infile);

    cout << tree->left_->left_->right_->data_;

//    while (len--) {
//        outfile << decode_symbol(tree, infile);
//    }
}

Node* deserialize_tree(ipd::bistream& infile)
{
    bool b;
    char c;
    infile.read(b);
    if (b == false) {
        // cout << "False! \n";
        c = infile.read_bits(b, 8);
        return new Node(c, 0);
    }
    else {
        // cout << "True!";
        return new Node(deserialize_tree(infile), deserialize_tree(infile));
    }
}

char decode_symbol(Node const* root, ipd::bistream& infile)
{
    Node const* tmp = root;
    if (tmp->is_leaf()){
        cout << tmp->data_;
        return tmp->data_;
    }
    bool b;
    infile.read(b);
    if(b) {
        decode_symbol(tmp->right_, infile);
    }
    else {
       decode_symbol(tmp->left_, infile);
    }
}

