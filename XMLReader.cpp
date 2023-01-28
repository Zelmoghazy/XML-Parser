#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string>
#include <stack>
#include <vector>
#include <iostream>

class XMLNode{
    public:
        char *tag;
        char *inner_text;
        int inner_text_size;
        XMLNode *parent;
        std::vector<XMLNode *> children;
        XMLNode(){
        }
        XMLNode(XMLNode* parent){
            this->parent = parent;
            this->tag = NULL;
            this->inner_text = NULL;
            this->inner_text_size = 0;
            if(parent){
                parent->children.push_back(this);
            }
        };
        ~XMLNode(){
            free(this->tag);
            free(this->inner_text);
            free(this);
        }
        // void add_child(XMLNode* child){
        //     children.push_back(child);
        // }
        XMLNode* XMLNode_at(int index){
            if(index > children.size()){
                printf("Wrong index for node this");
                exit(0);
            }
            else{
                return this->children[index];
            }
        }
        friend class XMLDocument;
};
class XMLDocument{
    public:
        XMLNode* root;

        XMLDocument(){
            this->root = NULL;
        }
        ~XMLDocument(){
            free(this->root);
        }
        bool checkBalance(std::string source, int n){
            int flag = 0;
            int opening = 0;
            int closing = 0;
            std::stack<int> s;
            for (int i = 0; i < n; i++)
            {
                if (source[i] == '<')
                {
                    s.push('<');
                    if (source[i + 1] == '/')
                    {
                        closing++;
                    }
                    else
                    {
                        opening++;
                    }
                }
                else if (source[i] == '>')
                {
                    if (s.empty())
                    {
                        flag++;
                    }
                    else if (s.top() == '<')
                    {
                        s.pop();
                    }
                }
            }
            if (flag != 0 || (closing - opening) != 0)
            {
                return false;
            }
            else if (s.empty() && (closing - opening) == 0)
            {
                return true;
            }
          return false; 
        }
};