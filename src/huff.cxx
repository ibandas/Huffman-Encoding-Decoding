#include <iostream>
#include "common.hxx"
#include "bit_io.hxx"
#include <map>
#include <queue>

// These are the default files to read from and write to when no
// command-line arguments are given:
// change "ascii" to "utf8" to see how this goes wrong with 8 bit files
#define DEFAULT_INFILE  "test-files/hamlet-ascii.txt"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.huff"

using namespace ipd;
using namespace std;


//(Done) 1. Read the file and count the frequency of each ASCII and store into a map
// (Done) 2. Sort the letters based on frequency
// (Done) 3. Create a vector of leaf nodes that is sorted in the same way as #2
// 4. Connect the two lowest frequency nodes together
// 5. Add the new internal node to the vector with a value of a combined frequency
// 6. Repeat #4-5 until there is one node left in the vector which is the root
// 7. Create a new mapping of 'key': ASCII and 'value': bitcode
// Note: You get the bitcode by traversing the tree with left meaning 0 and right meaning 1
// Example: 010 (left, right, left) is the 'A' ASCII value. So the key is 'A' and the value is bitcode '010'
// 8. We read the file again and turn every char into it's respective bitcode value based on the table above
// 9. Lastly, we serialize the tree with bits and store as metadeta at the top. This is done by
//    using preorder traversal where every branch is 1 and every leaf is 0 followed by the bit value of
//    the ASCII char
// Example: 110[A]0[B] (left left (left-leaf A) (right-leaf B))

int main(int argc, const char* argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    ifstream in(infile);
    assert_good(in, argv);

    bofstream out(outfile);
    assert_good(out, argv);

    // Build frequency map based off input file
    char c;
    map<char, int> frequencyMap;
    pair<map<char,int>::iterator,bool> put;

    while (in.read(&c, 1)) {
        put = frequencyMap.insert(pair<char, int>(c, 1));
        if (!put.second){
            frequencyMap[c] = frequencyMap[c] + 1;
        }
    }

    // Node struct
    struct node_ {
        char data_;
        int frequency_;
        node_(char data, int frequency) {
            data_ = data;
            frequency_ = frequency;
        };
    };

    // Comparison for frequency values
    struct node_cmp {
        bool operator()(node_ const& n1, node_ const& n2)
        {
            return n1.frequency_ < n2.frequency_;
        }
    };

    priority_queue <node_, vector<node_>, node_cmp> frequencyPQ;

    // Inserts into a PriorityQueue
    for (const auto &[k, v] : frequencyMap)
        frequencyPQ.push(node_(k, v));

    // Prints out the PQ
    while (!frequencyPQ.empty()) {
        node_ p = frequencyPQ.top();
        frequencyPQ.pop();
        cout << p.data_ << " " << p.frequency_ << "\n";
    }

    // Prints out size / amount of keys
    std::cout << "Size is " << frequencyMap.size() << '\n';
    return 0;
}
