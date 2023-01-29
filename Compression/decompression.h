#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>
#include <fstream>
#include <bitset>
#include <typeinfo>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Binary node 
struct BinaryNode;

// Convert the file from encoded to binary
string toBinary(string encoded);

// Insert every element to the binary tree
void insert (BinaryNode* root, char data, string code);

// Get the data as a binary tree
BinaryNode* getBinaryTree(string s);

// For Testing
void treeTraversal(BinaryNode* root);

// get the data by traversing the encoded data into the binary tree 
void getData(BinaryNode* root, string& code, int level, string& s);

// Decompress Function
void decompress(string compressed);

// Get the size of the file
std::ifstream::pos_type filesize(const char* filename);