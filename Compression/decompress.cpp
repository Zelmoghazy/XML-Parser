#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct BinaryNode{
      char data;
      BinaryNode *left,*right;

      BinaryNode(char data){
          left=right=NULL;
          this->data=data;
     }
};

// Insert every element to the binary tree
void insert (BinaryNode* root, char data, string code){
    // Base case
    if(code == "0"){
        BinaryNode* node = new BinaryNode(data);
        root->left = node;
    }
    else if(code == "1"){
        BinaryNode* node = new BinaryNode(data);
        root->right = node;
    }


    if(code[0] == '0' && code.size() != 1){
        if(!root->left){
            BinaryNode* node = new BinaryNode(code[0]);
            root->left = node;
        }
        insert(root->left, data, code.substr(1));
    }
    else if(code[0] == '1' && code.size() != 1){
        if(!root->right){
            BinaryNode* node = new BinaryNode(code[0]);
            root->right = node;
        }
        insert(root->right, data, code.substr(1));
    }
}

// Get the data as a binary tree
BinaryNode* BinaryData(string s){
    BinaryNode* root = new BinaryNode(NULL);
    int i = 0;
    int size = s.size();
    while(i != size-1){
        if(s[i] == ','){
            char data = s[i+1];
            i+=2;
            string code;
            while(s[i] != ','){
                string c (1,s[i]);
                code.append(""+c);
                i++;
            }
            insert(root, data, code);
        }
    }
    return root;
}

// For Testing
void treeTraversal(BinaryNode* root){
    // Base case
    if(!root->left && !root->right){
        cout << root->data << endl;
    }

    if(root->left){
        treeTraversal(root->left);
    }
    if(root->right){
        treeTraversal(root->right);
    }
}

// get the data by traversing the encoded data into the binary tree 
void getData(BinaryNode* root, string& code, int level, string& s){
    // Base case
    if(!root->left && !root->right){
        string c (1,root->data);
        s.append(c);
        return;
    }

    if(code[0] == '0' && root->left){
        code = code.substr(1);
        getData(root->left, code, level+1, s);
    }
    else if(code[0] == '1' && root->right){
        code = code.substr(1);
        getData(root->right, code, level+1, s);
    }
    
    if(level == -1 && code.size() != 0)
        getData(root, code, -1, s);
}

// Convert the file frim hexadecimal to binary
string toBinary(string hexadecimal){
    string out;
    for(int i = 0; i < hexadecimal.size(); i++){
        switch (hexadecimal[i])
        {
        case '0':
            out.append("0000");
            break;
        case '1':
            out.append("0001");
            break;
        case '2':
            out.append("0010");
            break;
        case '3':
            out.append("0011");
            break;
        case '4':
            out.append("0100");
            break;
        case '5':
            out.append("0101");
            break;
        case '6':
            out.append("0110");
            break;
        case '7':
            out.append("0111");
            break;
        case '8':
            out.append("1000");
            break;
        case '9':
            out.append("1001");
            break;
        case 'A':
            out.append("1010");
            break;
        case 'B':
            out.append("1011");
            break;
        case 'C':
            out.append("1100");
            break;
        case 'D':
            out.append("1101");
            break;
        case 'E':
            out.append("1110");
            break;
        case 'F':
            out.append("1111");
            break;
        default:
            break;
        }
    }
    return out;
}

// Decompress the file
void decompress(string compressed){
    // Open file
    ifstream infile;
    infile.open(compressed);
    
    // Get the data codes
    string dataCodes;
    getline(infile,dataCodes);
    
    // Put the data codes in the tree 
    BinaryNode* BinaryTree = BinaryData(dataCodes);
    
    //treeTraversal(HuffmanTree);

    // Get the encoded data
    string encodedData;
    getline(infile,encodedData);
    // convert it to binary
    encodedData = toBinary(encodedData);
    
    // Decode the data
    string data;
    getData(BinaryTree, encodedData, -1, data);
    
    // Finally, put the xml to the decompressed file 
    ofstream ofile;
    ofile.open("decompressed.txt");
    ofile << data;
    ofile.close();
}

int main(){
    decompress("compressed.txt");
    system("pause");
    return 0;
}
