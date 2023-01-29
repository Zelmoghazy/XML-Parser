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

// Minimum heap node
struct MinHeapNode;

// Compare the minimum value
struct compare;

// Get the data unique
string getUniqueData(string rawData);

// Get the frequency of data
void getFrequency(string uniqueData, string rawData, vector<int>& freq);

// Make the huffman tree
MinHeapNode* HuffmanCodes(string data, vector<int>& freq, int size);

// Put the code for every character
string printCodes(struct MinHeapNode* root,string code);

// Encode the XML file to binary by huffman algorithm
string encodeDataToBinary(string rawData, struct MinHeapNode* root);

// Get the code a single character
string getCode(char data,struct MinHeapNode* root);

// Compress the binary data
void compressBinaryData(string binary, ofstream& f);

// Compress Function
void compress(string sample);

// Get the size of the file
std::ifstream::pos_type filesize(const char* filename);

