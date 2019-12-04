#include <iostream>
#include "common.hxx"
#include "bit_io.hxx"
#include "huff_tree.hxx"
#include <map>
#include <queue>

// These are the default files to read from and write to when no
// command-line arguments are given:
// change "ascii" to "utf8" to see how this goes wrong with 8 bit files
#define DEFAULT_INFILE  "test-files/hamlet-ascii.txt"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.huff"
// #define DEFAULT_OUTFILE "test-files/hamlet-ascii-test.txt"

using namespace ipd;
using namespace std;


//(Done) 1. Read the file and count the frequency of each ASCII and store into a map
// (Done) 2. Sort the letters based on frequency
// (Done) 3. Create a vector of leaf nodes that is sorted in the same way as #2
// (Done) 4. Connect the two lowest frequency nodes together
// (Done) 5. Add the new internal node to the vector with a value of a combined frequency
// (Done) 6. Repeat #4-5 until there is one node left in the vector which is the root
// (Done) 7. Create a new mapping of 'key': ASCII and 'value': bitcode
// Note: You get the bitcode by traversing the tree with left meaning 0 and right meaning 1
// Example: 010 (left, right, left) is the 'A' ASCII value. So the key is 'A' and the value is bitcode '010'
// (Done) 8. We read the file again and turn every char into it's respective bitcode value based on the table above
// (Done) 9. Lastly, we serialize the tree with bits and store as metadeta at the top. This is done by
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

    Tree heap = Tree(in, out);

    // Converts each character from the infile to it's respective bit code into the huff file
//    ifstream in_convert_bit(infile);
//    while (in_convert_bit.read(&c, 1)) {
//        for (int i = 0; i < bitMap[c].size(); i++) {
//            out.write(bitMap[c][i]);
//        }
//    }


    // Puff

}