#include <iostream>
#include "common.hxx"
#include "bit_io.hxx"

// These are the default files to read from and write to when no
// command-line arguments are given:
// change "ascii" to "utf8" to see how this goes wrong with 8 bit files
#define DEFAULT_INFILE  "test-files/hamlet-ascii.txt"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.huff"

using namespace ipd;
using namespace std;


// 1. Count the frequency of each ASCII
// 2. Sort the letters based on frequency
// 3. Create a list of leaf nodes that is sorted in the same way as #2
// 4. Connect the two lowest frequency nodes together
// 5. Add the new internal node to the list
// 6. Repeat #4-5
// Note: Going left gives 0 bit and going right gives 1 bit
// 7. Serialize the tree with bits and store as metadeta at the top

int main(int argc, const char* argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    ifstream in(infile);
    assert_good(in, argv);

    bofstream out(outfile);
    assert_good(out, argv);
    return 0;
}
