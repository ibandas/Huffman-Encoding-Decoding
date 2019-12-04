#include "huff_tree.hxx"
#include <map>
#include <queue>

using namespace std;
using namespace ipd;

void encode(std::istream &infile, ipd::bostream &outfile)
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
    build_code_word_table(huffman_tree, {}, cwt);

    outfile.write_bits(len, CHAR_BIT * sizeof len);
    serialize_tree(huffman_tree, outfile);
    encode_stream(cwt, infile, outfile);
}

void Tree::build_Tree()
{
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
}

void decode(ipd::bistream& infile, std::ostream& outfile)
{
    size_t len;

    infile.read_bits(len, CHAR_BIT * sizeof len);
    auto tree = deserialize_tree(infile);

    while (len--) {
        outfile << decode_symbol(tree, infile);
    }
}

// TODO
Node* build_tree(frequency_table_t const&)
{
    return nullptr;
}

// TODO
void build_code_word_table(Node const*,
                           code_word_t const&,
                           code_word_table_t& cwt)
{ }

void serialize_tree(Node const* node, bostream &outfile){
    if (node->is_leaf()){
        outfile.write(false);
        outfile.write_bits(node->data_, 8);
    } else {
        outfile.write(true);
        serialize_tree(node->left_, outfile);
        serialize_tree(node->right_, outfile);
    }
}

// TODO
void encode_stream(code_word_table_t const&,
                   std::istream&, ipd::bostream&)
{ }

// TODO
Node* deserialize_tree(ipd::bistream&)
{
    return nullptr;
}

// TODO
char decode_symbol(Node const*, ipd::bistream&)
{
    return 0;
}

