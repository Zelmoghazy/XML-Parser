#include "compression.h"

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

// Compare the minimum value
struct compare{

    bool operator()(MinHeapNode *l,MinHeapNode *r)
    {
        return (l->freq>r->freq);
    }
};


// Get the data unique
string getUniqueData(string rawData){
    string uniqueData;
    for(int i = 0; i < rawData.size(); i++){
        if(uniqueData.find(rawData[i]) == string::npos){
            uniqueData.push_back(rawData[i]);
        }
    }
    return uniqueData;
}

// Get the frequency of data
void getFrequency(string uniqueData, string rawData, vector<int>& freq){
    for(int i = 0; i < uniqueData.size(); i++){
        int x = count(rawData.begin(), rawData.end(), uniqueData[i]);
        freq.push_back(x);
    }
}

// Put the code for every character
string printCodes(struct MinHeapNode* root,string code){
   static string encodedDataInfo;
   //If root is Null then return.
   if(!root){
       return "";
   }

   if(!root->left && !root->right){
       string s(1,root->data); 
       encodedDataInfo.append(','+s + code);
       root->code = code;
   }

   printCodes(root->left,code+"0");
   printCodes(root->right,code+"1");
   return encodedDataInfo;
}


// Make the huffman tree
MinHeapNode* HuffmanCodes(string data, vector<int>& freq, int size){

struct MinHeapNode *left,*right,*top;

//create a min heap.
priority_queue<MinHeapNode*,vector<MinHeapNode*>,compare> minheap;
    

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

// Get the code a single character
string getCode(char data,struct MinHeapNode* root){
    //If root is Null then return.
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

// Encode the XML file to binary by huffman algorithm
string encodeDataToBinary(string rawData, struct MinHeapNode* root){
    //If root is Null then return.
    if(!root){
        return "";
    }

    string encodedData;
    for(int i = 0; i < rawData.size();i++){
        encodedData.append(getCode(rawData[i],root));
    }
    return encodedData;
}


// Compress the binary data
void compressBinaryData(string binary, ofstream& f){
    string substring;
    for(int i = 0; i < binary.size(); i+=7){
        if(binary.size() - i < 7){
            substring = binary.substr(i,binary.size() - i);
            break;
        }
       bitset<7>  b(binary.substr(i,7));
        unsigned char c = static_cast<unsigned char>(b.to_ulong());
        c = c+30;
        f<<c;
    }
    f<<substring<<substring.size();
}

// Compress Function
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
    string encodedData  = encodeDataToBinary(data,HuffmanTree);

    // Finally, put the data codes and the encoded data as a compressed file 
    ofstream ofile;
    ofile.open("compressed.huf");
    ofile << dataCodes << "," << endl;
    compressBinaryData(encodedData, ofile);
    
    ofile.close();
}

// Get the size of the file
std::ifstream::pos_type filesize(const char* filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    return in.tellg(); 
}

/*int main(){
    // To compress
        // Get the time
    auto start = high_resolution_clock::now();
    compress("sample.xml");
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<microseconds>(stop - start);
    
        // Print the time duration and size
    cout << "Minified XML file size (bytes)       : " << filesize("sample.xml") << endl;
    cout << "Compressed XML file size (bytes)     : " << filesize("compressed.huf") << endl;
    
    cout << "Compress time duration (microseconds): " << duration.count() << endl;

    /*_____________________________________Compression For Testing_____________________________________*/
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

    
    cout << "########################## encodeDataToBinary() Testing ############################" << endl;
    cout << "Raw data : " << data << endl;
    string encodedData  = encodeDataToBinary(data,HuffmanTree);
    cout << "Encoded data : " << toHexadecimal(encodedData) << endl;
    

    system("pause");
    return 0;
}*/