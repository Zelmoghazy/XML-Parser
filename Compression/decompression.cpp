#include "decompression.h"

// Binary node 
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
    //If root is Null then return.
    if(!root){
        return;
    }

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
BinaryNode* getBinaryTree(string s){
    BinaryNode* root = new BinaryNode('$');
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
    //If root is Null then return.
    if(!root){
        return;
    }

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
    //If root is Null then return.
    if(!root){
        return;
    }

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

// Convert the file from encoded to binary
string toBinary(string encoded){
    string out;
    int condition = encoded[encoded.size()-1]-48;
    int a = encoded.size();
    for(int i = 0; i < encoded.size()-1; i++){
        if(i < encoded.size()-condition-1){
            encoded[i] = encoded[i] - 30;
            std::bitset<7> b(encoded[i]);
            out.append(b.to_string());
        }else{
            string s(1,encoded[i]);
            out.append(s);
        }
    }
    return out;
}

// Decompress Function
void decompress(string compressed){
    // Open file
    ifstream infile;
    infile.open(compressed);

    string dataCodes;
    string encodedData;

    if(infile.is_open()){
        // Get the data codes
        getline(infile,dataCodes);
        
        // Get the encoded data
        getline(infile, encodedData);

        infile.close();
    }
    else{
        cout << "Err" << endl;
    }
    
    // Put the data codes in the tree 
    BinaryNode* BinaryTree = getBinaryTree(dataCodes);

    
    
    // convert it to binary
    encodedData = toBinary(encodedData);
    
    // Decode the data
    string data;
    getData(BinaryTree, encodedData, -1, data);
    
    // Finally, put the xml to the decompressed file 
    ofstream ofile;
    ofile.open("decompressed.xml");
    ofile << data;
    ofile.close();
}

int main(){
    // To decompress
    decompress("compressed.huf");
    
    system("pause");
    return 0;
}


