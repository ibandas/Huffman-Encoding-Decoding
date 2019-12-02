#include "huff_tree.hxx"

using namespace std;
using namespace ipd;

HuffTree::HuffTree() {

}

HuffTree::HuffTree(map<char, unsigned> data_map) {
    // Call a sorting function on the map before inserting into Tree
    sort_map(data_map);
    // Iterate through map, and call insert
}

void HuffTree::insert(char data, unsigned freq) {

}

map<char, unsigned> HuffTree::sort_map(map<char, unsigned>) {
    return map<char, unsigned int>();
}