#include <iostream>
#include <algorithm>
#include <queue>
#include <cstring>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

// Minimum heap node
struct MinHeapNode{
      char data;
      int freq;
      string code;

      MinHeapNode *left,*right;

      MinHeapNode(char data,int freq){
          left=right=NULL;
          this->data=data;
          this->freq=freq;
     }
};

// compare the minimum value
struct compare{

    bool operator()(MinHeapNode *l,MinHeapNode *r)
    {
        return (l->freq>r->freq);
    }
};


// Get the data unique
string getUniqueData(string data){
    string uniqueData;
    for(int i = 0; i < data.size(); i++){
        if(uniqueData.find(data[i]) == string::npos){
            uniqueData.push_back(data[i]);
        }
    }
    return uniqueData;
}

// Get the frequency of data
void getFrequency(string uniqueData, string data, vector<int>& v){
    for(int i = 0; i < uniqueData.size(); i++){
        int x = count(data.begin(), data.end(), uniqueData[i]);
        v.push_back(x);
    }
}

string printCodes(struct MinHeapNode* root,string str){
   static string encodedDataInfo;
   //If root is Null then return.
   if(!root){
       return "";
   }
   //If the node's data is not '$' that means it's not an internal node and print the string.
   if(!root->left && !root->right){
       string s(1,root->data); 
       encodedDataInfo.append(s +":"+ str + ",");
       root->code = str;
   }

   printCodes(root->left,str+"0");
   printCodes(root->right,str+"1");
   return encodedDataInfo;
}

MinHeapNode* HuffmanCodes(string data, vector<int>& freq, int size){

struct MinHeapNode *left,*right,*top;

//create a min heap.
priority_queue<MinHeapNode*,vector<MinHeapNode*>,compare> minheap;

    //get the data and frequency
    
// For each character create a leaf node and insert each leaf node in the heap.
    

for(int i=0;i<size;i++){
    minheap.push(new MinHeapNode(data[i], freq.at(i)));
 }

 //Iterate while size of min heap doesn't become 1
 while(minheap.size()!=1){
     //Extract two nodes from the heap.
     left = minheap.top();
     minheap.pop();

     right = minheap.top();
     minheap.pop();


     MinHeapNode* tmp = new MinHeapNode('$',left->freq+right->freq);
     tmp->left = left;
     tmp->right = right;

     minheap.push(tmp);
 }
    //Calling function to print the codes.
    return minheap.top();
}

string getCode(char data,struct MinHeapNode* root){
    if(!root){
        return "";
    }
    if(root->data == data){
        return root->code;
    }
    if(getCode(data, root->left) != ""){
       return  getCode(data, root->left);
    }
       return  getCode(data, root->right);
}

string encodeData(string data, struct MinHeapNode* root){
    string encodedData;
    for(int i = 0; i < data.size();i++){
        encodedData.append(getCode(data[i],root));
    }
    return encodedData;
}

string toHexadecimal(string binary){
    string substring,out;
    for(int i = 0; i < binary.size(); i+=4){
        substring = binary.substr(i,4);
        int sub = atoi(substring.c_str());
        switch (sub)
        {
        case 0000:
            out.append("0");
            break;
        case 0001:
            out.append("1");
            break;
        case 0010:
            out.append("2");
            break;
        case 0011:
            out.append("3");
            break;
        case 0100:
            out.append("4");
            break;
        case 0101:
            out.append("5");
            break;
        case 0110:
            out.append("6");
            break;
        case 0111:
            out.append("7");
            break;
        case 1000:
            out.append("8");
            break;
        case 1001:
            out.append("9");
            break;
        case 1010:
            out.append("A");
            break;
        case 1011:
            out.append("B");
            break;
        case 1100:
            out.append("C");
            break;
        case 1101:
            out.append("D");
            break;
        case 1110:
            out.append("E");
            break;
        case 1111:
            out.append("F");
            break;
        default:
            break;
        }
    }
    return out;
}

void compress(string sample){
    string data;
    ifstream f;
    // Open the XML file
    f.open(sample);
    if(f.is_open()){
        getline(f,data);
        f.close();
    }
    else{
        cout << "Err" << endl;
    }

    // Get the data unique
    string uniqueData = getUniqueData(data);
    
    // Get the frequency of data
    vector<int> frequencyData;
    getFrequency(uniqueData, data, frequencyData);

    // Get the Tree of data codes
    MinHeapNode* HuffmanTree = HuffmanCodes(uniqueData, frequencyData, uniqueData.size());

    // Get the data codes as a string
    string dataCodes;
    dataCodes.append(printCodes(HuffmanTree,""));

    // Get the data encoded
    string encodedData  = encodeData(data,HuffmanTree);

    // Finally, put the data codes and the encoded data as a compressed file 
    ofstream ofile;
    ofile.open("compressed.txt");
    ofile << dataCodes << endl << toHexadecimal(encodedData);
    ofile.close();
}

std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

int main(){
    // Actual compress
    auto start = high_resolution_clock::now();
    compress("sample.xml");
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    
    cout << "Minified XML file size (bytes)       : " << filesize("sample.xml") << endl;
    cout << "Compressed XML file size (bytes)     : " << filesize("compressed.txt") << endl;
    
    cout << "Compress time duration (microseconds): " << duration.count() << endl;
    
    /*___________________________________________For Testing_____________________________________*/
    /*
    cout <<"########################## getUniqueData() Testing ############################" << endl;

    string data = "<users><user><id>1</id><name>Ahmed Ali</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>economy</topic><topic>finance</topic></topics></post><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>solar_energy</topic></topics></post></posts><followers><follower><id>2</id></follower><follower><id>3</id></follower></followers></user><user><id>2</id><name>Yasser Ahmed</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>education</topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user><user><id>3</id><name>Mohamed Sherif</name><posts><post><body>Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat.</body><topics><topic>sports</topic></topics></post></posts><followers><follower><id>1</id></follower></followers></user></users>";
    string uniqueData = getUniqueData(data);

    cout << "Unique data : " << uniqueData;
    cout << "\n" <<  endl;
    
    //sort the unique data alphabetically
    sort(uniqueData.begin(),uniqueData.end());
    

   

    cout <<"########################## getFrequency() Testing ############################" << endl;

    vector<int> frequencyData;
    getFrequency(uniqueData, data, frequencyData);

     for(int i = 0; i < uniqueData.size(); i++){
        cout << uniqueData[i] << ":" << frequencyData.at(i) << endl;
    }
    cout << "\n" <<  endl;

    cout << "########################## HuffmanCodes() Testing ############################" << endl;
    
    MinHeapNode* HuffmanTree = HuffmanCodes(uniqueData, frequencyData, uniqueData.size());
    string dataCodes;
    dataCodes.append(printCodes(HuffmanTree,""));
    cout << dataCodes << endl;

    
    cout << "########################## encodeData() Testing ############################" << endl;
    cout << "Raw data : " << data << endl;
    string encodedData  = encodeData(data,HuffmanTree);
    cout << "Encoded data : " << toHexadecimal(encodedData) << endl;
    */

    system("pause");
    return 0;
}