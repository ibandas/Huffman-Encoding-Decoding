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

// using namespace ipd;
using namespace std;

int main(int argc, const char* argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    ifstream in(infile);
    assert_good(in, argv);

    bofstream out(outfile);
    assert_good(out, argv);

    encode_huff(in, out);

}