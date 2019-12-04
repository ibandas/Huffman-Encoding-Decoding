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
// #define DEFAULT_OUTFILE "test-files/hamlet-ascii.txt.huff"
#define DEFAULT_OUTFILE "test-files/hamlet-ascii-test.txt"
#define internal_node '\0'

using namespace ipd;
using namespace std;


//(Done) 1. Read the file and count the frequency of each ASCII and store into a map
// (Done) 2. Sort the letters based on frequency
// (Done) 3. Create a vector of leaf nodes that is sorted in the same way as #2
// (Done) 4. Connect the two lowest frequency nodes together
// (Done) 5. Add the new internal node to the vector with a value of a combined frequency
// (Done) 6. Repeat #4-5 until there is one node left in the vector which is the root
// (Done - Kind of) 7. Create a new mapping of 'key': ASCII and 'value': bitcode
// TODO: Switch from strings to bools
// Note: You get the bitcode by traversing the tree with left meaning 0 and right meaning 1
// Example: 010 (left, right, left) is the 'A' ASCII value. So the key is 'A' and the value is bitcode '010'
// 8. We read the file again and turn every char into it's respective bitcode value based on the table above
// 9. Lastly, we serialize the tree with bits and store as metadeta at the top. This is done by
//    using preorder traversal where every branch is 1 and every leaf is 0 followed by the bit value of
//    the ASCII char
// Example: 110[A]0[B] (left left (left-leaf A) (right-leaf B))

void getNodes(Node *head, string path, vector<Node*> &nodesList){
    if (!head){
        return;
    }

    if (head->data_ != internal_node) {
        head->code_ = path;
        nodesList.push_back(head);
    }

    getNodes(head->left_, path + "0", nodesList);

    getNodes(head->right_, path + "1", nodesList);

}

void printList(vector<Node*> &nodesList){
    Node *tmp;

    cout << "\nChar \t" << "Freq \t" << "Code \n\n";

    for (int i = 0; i < nodesList.size(); i++){
        cout << nodesList[i]->data_ << "\t " << nodesList[i]->freq_ << "\t " << nodesList[i]->code_ << "\n";
    }
}

int main(int argc, const char* argv[])
{
    const char *infile, *outfile;

    get_file_names(argc, argv, infile, outfile,
                   DEFAULT_INFILE, DEFAULT_OUTFILE);

    ifstream in(infile);
    assert_good(in, argv);

    // bofstream out(outfile);
    ofstream out(outfile);
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

    // Comparison for frequency values
    struct node_cmp {
        bool operator()(Node* const& n1, Node* const& n2)
        {
            return n1->freq_ < n2->freq_;
        }
    };

    priority_queue <Node *, vector<Node *>, node_cmp> frequencyPQ;

    // Inserts into a PriorityQueue
    for (const auto &[k, v] : frequencyMap)
        frequencyPQ.push(new Node(k, v));



    // Create tree
    while (frequencyPQ.size() > 1) {
        // Remove first lowest
        Node* min1 = frequencyPQ.top();
        frequencyPQ.pop();

        // Remove second lowest
        Node* min2 = frequencyPQ.top();
        frequencyPQ.pop();

        // Merge into an internal node
        Node * new_internal_node = new Node(internal_node, min1->freq_ + min2->freq_);
        new_internal_node->left_ = min1;
        new_internal_node->right_ = min2;

        // Push the new internal node back into the frequencyPQ
        frequencyPQ.push(new_internal_node);
    }

    // Build frequency map based off input file
    map<char, string> bitMap;
    pair<map<char, string>::iterator,bool> bitPut;

     vector<Node*> nodesList;
     getNodes(frequencyPQ.top(), string(""), nodesList);

    for (int i = 0; i < nodesList.size(); i++){
        bitMap.insert(pair<char, string>(nodesList[i]->data_, nodesList[i]->code_));
    }

    ifstream in_test(infile);
    while (in_test.read(&c, 1)) {
        out << bitMap[c];
    }

//     printList(nodesList);

    return 0;
}


// Prints out the PQ
//    while (!frequencyPQ.empty()) {
//        Node* p = frequencyPQ.top();
//        frequencyPQ.pop();
//        cout << p->data_ << " " << p->freq_ << "\n";
//    }

// Prints out size / amount of keys
// std::cout << "Size is " << frequencyMap.size() << '\n';