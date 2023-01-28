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
        
        bool XMLDocument_load(const char *path)
        {
            FILE *file = fopen(path, "r");
            // Check if file loads properly
            if (!file)
            {
                // output to stderr to seperate error messages from regular output
                fprintf(stderr, "Couldnt load file");
                return false;
            }
            std::string source;
            int size = 1;
            char ch;
            // count number of new lines
            //  a new line is represented by two bytes in a text file,
            //  but when reading it only yields a single '\n' character.
            while (file)
            {
                ch = fgetc(file);
                if (ch == EOF)
                    break;
                source.push_back(ch);
                size++;
            }
            fread(&source[0], 1, size, file);
            fclose(file);
            /*-----------------------------------------------------*/

            // null terminate
            source[size] = '\0';

            // check consistency
            if (checkBalance(source, size))
            {
                printf("the xml is balanced\n");
            }
            else
            {
                printf("The XML is not balanced\n");
                // Abdo add return
            }

            // Create new root node (no parent)
            //------------------------------//
            this->root = new XMLNode(root);
            // char array to extract information (Vector in c++)
            char buffer[2560];
            // index to control the buffer array
            int buffer_index = 0;
            // index of source array
            int i = 0;

            // first node to work with is the doc root node
            XMLNode *curr_node = this->root;

            // Parsing loop (Seperate function)
            // as long as we dont reach end of source array
            while (source[i] != '\0')
            {
                // if we find an open tag
                if (source[i] == '<')
                {
                    // Reset the buffer
                    buffer[buffer_index] = '\0';
                    // Inner Text
                    // If buffer index is not zero then there is inner text
                    if (buffer_index > 0)
                    {
                        // Append extracted inner text to our buffer and make note of size
                        curr_node->inner_text_size += buffer_index;
                        curr_node->inner_text = strdup(buffer);
                        // reset buffer index
                        buffer_index = 0;
                    }

                    // End of node
                    // if we find a '/' after '<' we are at the end of node
                    if (source[i + 1] == '/')
                    {
                        // move two positions <,/
                        i += 2;
                        // while we havent reached end tag append to buffer
                        while (source[i] != '>')
                        {
                            buffer[buffer_index++] = source[i++];
                        }
                        // once we reach end tag add null termination to denote the extracted string
                        buffer[buffer_index] = '\0';
                        // If current Node is NULL it means only end tag is there
                        if (!curr_node->tag)
                        {
                            fprintf(stderr, "Missing start tag\n");
                            free(curr_node);
                            return false;
                        }
                        // Check if tags match
                        // strcmp returns 0 if they match
                        if (strcmp(curr_node->tag, buffer))
                        {
                            fprintf(stderr, "Mismatched tags (%s != %s)\n", curr_node->tag, buffer);
                            char choice;
                            printf("Do you want to correct the error and match with %s tag: enter 'y' or 'n' ", curr_node->tag);
                            scanf(" %c", &choice);
                            // Correct mismatched tags
                            if (choice == 'y')
                            {
                                // buffer = strdup(curr_node->tag);
                            }
                            else
                            {
                                free(curr_node);//???????????????????
                                return false;
                            }
                        }
                        // If they match we are done move to parent
                        curr_node = curr_node->parent;
                        buffer_index = 0;
                        i++;
                        continue;
                    }
                    // Comments and special nodes <!--->
                    if (source[i + 1] == '!')
                    {
                        while (source[i] != '>')
                        {
                            if (buffer[buffer_index - 1] == ' ')
                            {
                                // dont pickup space
                                buffer_index--;
                                continue;
                            }
                            buffer[buffer_index++] = source[i++];
                        }
                        buffer[buffer_index] = '\0';
                        // Comments
                        if (buffer[buffer_index - 1] == '-' && buffer[buffer_index - 2] == '-')
                        {
                            continue;
                        }
                    }

                    // Create a new node with the current node being the parent
                    //------------------------------//
                    curr_node = new XMLNode(curr_node);

                    // Start Tag
                    // increment where we are in the source
                    i++;
                    // While we havent reached the end tag
                    while (source[i] != '>')
                    {
                        // add the data to the buffer
                        buffer[buffer_index++] = source[i++];
                        // <tag  >
                        // if we find a space and we still dont have a tag
                        // skip space and set tag
                        if (source[i] == ' ' && !curr_node->tag)
                        {
                            // denote the end of extracted string
                            buffer[buffer_index] = '\0';
                            // set the tag
                            curr_node->tag = strdup(buffer);
                            // reset buffer index
                            buffer_index = 0;
                            i++;
                            continue;
                        }
                        // if last character is space
                        // ignore other spaces
                        if (buffer[buffer_index - 1] == ' ')
                        {
                            // dont pickup space
                            buffer_index--;
                            continue;
                        }
                    }
                    //<tag>inner text
                    // set tag name if it doesnt exist
                    buffer[buffer_index] = '\0';
                    if (!curr_node->tag)
                    {
                        curr_node->tag = strdup(buffer);
                    }
                    // Reset
                    buffer_index = 0; // reset
                    i++;
                    continue;
                }
                else
                // if not inside tag
                // keep appending inner text to the buffer
                {
                    if (source[i] == '\n')
                    {
                        i++;
                        // found white space after newline
                        while (source[i] == ' ')
                        {
                            // ignore white space dont add to buffer
                            i++;
                        }
                    }
                    else
                    {
                        buffer[buffer_index++] = source[i++];
                    }
                }
            }
            // if current node is NULL and the index is not zero then there is an outside text
            if (buffer_index > 0 && curr_node->tag == NULL)
            {
                fprintf(stderr, "text outside of document\n");
                free(curr_node);//????????????
                return false;
            }
            else if (curr_node->tag)
            {
                fprintf(stderr, "missing end tag\n");
                curr_node = curr_node->parent;
                // return false;
            }
            this->root = curr_node;
            return true;
        }
        
};