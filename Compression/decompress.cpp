#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>

using namespace std;

struct MinHeapNode{
      char data;
      int freq;
      string code;

      MinHeapNode *left,*right;

      MinHeapNode(char data){
          left=right=NULL;
          this->data=data;
     }
};

void insert (MinHeapNode* root, char data, string code){
    // Base case
    if(code == "0"){
        MinHeapNode* node = new MinHeapNode(data);
        root->left = node;
    }
    else if(code == "1"){
        MinHeapNode* node = new MinHeapNode(data);
        root->right = node;
    }


    if(code[0] == '0' && code.size() != 1){
        if(!root->left){
            MinHeapNode* node = new MinHeapNode(code[0]);
            root->left = node;
        }
        insert(root->left, data, code.substr(1));
    }
    else if(code[0] == '1' && code.size() != 1){
        if(!root->right){
            MinHeapNode* node = new MinHeapNode(code[0]);
            root->right = node;
        }
        insert(root->right, data, code.substr(1));
    }
}

MinHeapNode* HuffmanData(string s){
    MinHeapNode* root = new MinHeapNode(NULL);
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


void treeTraversal(MinHeapNode* root){
    // Base case
    if(!root->left && !root->right){
        cout << root->data << endl;
    }

    if(root->left)
        treeTraversal(root->left);
    if(root->right)
        treeTraversal(root->right);
}

void getData(MinHeapNode* root, string& code, int level, string& s){
    // Base case
    if(!root->left && !root->right){
        string c (1,root->data);
        s.append(c);
    }

    if(code[0] == '0' && root->left){
        code = code.substr(1);
        getData(root->left, code, level+1, s);
    }
    if(code[0] == '1' && root->right){
        code = code.substr(1);
        getData(root->right, code, level+1, s);
    }
    
    if(level == 0 && code.size() != 0)
        getData(root, code, 0, s);
}

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

int main(){
    // Open file
    ifstream infile;
    infile.open("compressed.txt");
    
    // Get the data codes
    string s;
    getline(infile,s);
    
    // Put the data codes in the tree 
    MinHeapNode* HuffmanTree = HuffmanData(s);
    
    // Get the encoded data
    string encodedData =  "101110101";
    //getline(infile,encodedData);
    // convert it to binary
    
    
    // Decode the data
    string data;
    getData(HuffmanTree, encodedData, 0, data);
    
    // Print thr data
    cout << data << endl;
    system("pause");
    return 0;
}
