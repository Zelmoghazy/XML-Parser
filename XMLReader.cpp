#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string>
#include <stack>
#include <vector>
#include <iostream>

class XMLNode
{
public:
    char *tag;
    char *inner_text;
    int inner_text_size;
    XMLNode *parent;
    std::vector<XMLNode *> children;
    XMLNode()
    {
    }
    XMLNode(XMLNode *parent)
    {
        this->parent = parent;
        this->tag = NULL;
        this->inner_text = NULL;
        this->inner_text_size = 0;
        if (parent)
        {
            parent->children.push_back(this);
        }
    };
    ~XMLNode()
    {
        free(this->tag);
        free(this->inner_text);
        free(this);
    }
    // void add_child(XMLNode* child){
    //     children.push_back(child);
    // }
    XMLNode *XMLNode_at(int index)
    {
        if (index > children.size())
        {
            printf("Wrong index for node this");
            exit(0);
        }
        else
        {
            return this->children[index];
        }
    }
    friend class XMLDocument;
};
class XMLDocument
{
public:
    XMLNode *root;

    XMLDocument()
    {
        this->root = NULL;
    }
    ~XMLDocument()
    {
        free(this->root);
    }
    bool checkBalance(std::string source, int n)
    {
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
                    return false;
                }
                else if (s.top() == '<')
                {
                    s.pop();
                }
            }
        }
        if (s.empty() && (closing - opening) == 0)
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
        
         // check consistency
        if (checkBalance(source, size))
        {
            printf("the xml is balanced\n");
        }
        else
        {
            printf("The XML is not balanced\n");
            return false;
        }

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
                        return false;
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
            free(curr_node); //????????????
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
    void Format(FILE *file, XMLNode *node, int indent, int times)
    {
        for (int i = 0; i < node->children.size(); i++)
        {
            XMLNode *child = node->children[i];
            if (times > 0)
            {
                fprintf(file, "%0*s", indent * times, " ");
            }
            fprintf(file, "<%s", child->tag);
            // no inner text
            if (child->children.size() == 0 && !child->inner_text)
            {
                fprintf(file, " />\n");
            }
            else
            {
                fprintf(file, ">");
                if (child->children.size() == 0)
                {
                    if (child->inner_text_size > 200) // if inner text size is large make new line
                    {
                        fprintf(file, "\n%0*s%s\n%0*s</%s>\n", (indent * (times + 1)), " ", child->inner_text, (indent * (times)), " ", child->tag);
                    }
                    else // same line if inner text small
                    {
                        fprintf(file, "%s</%s>\n", child->inner_text, child->tag);
                    }
                }
                else
                {
                    fprintf(file, "\n");
                    Format(file, child, indent, times + 1);
                    if (times > 0)
                    {
                        fprintf(file, "%0*s", indent * times, " ");
                    }
                    fprintf(file, "</%s>\n", child->tag);
                }
            }
        }
    }
    void Minify(FILE *file, XMLNode *node)
    {
        for (int i = 0; i < node->children.size(); i++)
        {
            XMLNode *child = node->children[i];
            fprintf(file, "<%s", child->tag);
            // no inner text
            if (child->children.size() == 0 && !child->inner_text)
            {
                fprintf(file, " />");
            }
            else
            {
                fprintf(file, ">");
                if (child->children.size() == 0)
                {
                    fprintf(file, "%s</%s>", child->inner_text, child->tag);
                }
                else
                {
                    Minify(file, child);
                    fprintf(file, "</%s>", child->tag);
                }
            }
        }
    }

    bool XMLDocument_write(const char *path, const char choice)
    {
        FILE *file = fopen(path, "w");
        if (!file)
        {
            fprintf(stderr, "Cannot open file to write");
            return false;
        }
        if (choice == 'f')
        {
            this->Format(file, this->root, 3, 0); // identation = 3 spaces (can be changed)
        }
        else if (choice == 'm')
        {
            Minify(file, this->root);
        }
        fclose(file);
        return true;
    }
    void print_ID(XMLNode *user)
    {
        XMLNode *id = user->children[0];
        printf("%s: %s\n", id->tag, id->inner_text);
    }
    void print_Name(XMLNode *user)
    {
        XMLNode *name = user->children[1];
        printf("%s: %s\n", name->tag, name->inner_text);
    }
    void print_Posts(XMLNode *user)
    {
        XMLNode *post = user->children[2];
        std::vector<XMLNode *> posts = post->children;
        for (int i = 0; i < posts.size(); i++)
        {
            XMLNode *field = posts[i];
            printf("--------------------\n");
            printf("%s : %d\n", field->tag, i + 1);
            printf("--------------------\n");
            XMLNode *body = field->children[0];
            printf("%s: %s\n", body->tag, body->inner_text);
            XMLNode *topics = field->children[1];
            std::vector<XMLNode *> topic = topics->children;
            for (int j = 0; j < topic.size(); j++)
            {
                XMLNode *mawdoo = topic[j];
                printf("%s: %s\n", mawdoo->tag, mawdoo->inner_text);
            }
        }
    }
    void print_Followers(XMLNode *user)
    {
        XMLNode *follower = user->children[3];
        std::vector<XMLNode *> followers = follower->children;
        for (int i = 0; i < followers.size(); i++)
        {
            XMLNode *field = followers[i];
            printf("%s: %d\n", field->tag, i + 1);
            XMLNode *id = field->children[0];
            printf("%s: %s\n", id->tag, id->inner_text);
        }
    }
    char *get_Follower_id(XMLNode *user, int index)
    {
        XMLNode *follower = user->children[3];
        std::vector<XMLNode *> followers = follower->children;
        if (followers.size() >= index)
        {
            XMLNode *field = followers[index];
            XMLNode *id = field->children[0];
            return id->inner_text;
        }
        else
        {
            return (char *)-1;
        }
    }
    XMLNode *getUser_ID(XMLNode *users, const char *ID)
    {
        for (int i = 0; i < users->children.size(); i++)
        {
            XMLNode *user = users->children[i];
            XMLNode *id = user->children[0];
            if (!strcmp(id->inner_text, ID))
            {
                return users->children[i];
            }
        }
        return NULL;
    }
    XMLNode *getUser_Name(XMLNode *users, const char *Name)
    {
        for (int i = 0; i < users->children.size(); i++)
        {
            XMLNode *user = user->children[i];
            XMLNode *name = user->children[1];
            if (!strcmp(name->inner_text, Name))
            {
                return users->children[i];
            }
        }
        return NULL;
    }
};
bool are_same_children(XMLNode *parent)
{
    int mark = 0;
    for (int i = 0; i < parent->children.size - 1; i++)
    {
        if (strcmp(parent->children.data[i]->tag, parent->children.data[i + 1]->tag))
        {
            return false;
        }
        else
        {
            if (mark == 0)
            {
                parent->children.data[i]->flag = 2;
                if (i == (int)parent->children.size - 2)
                {
                    parent->children.data[i + 1]->flag = 4;
                }
                else
                {
                    parent->children.data[i + 1]->flag = 1;
                }
                mark++;
            }
            else
            {
                parent->children.data[i]->flag = 1;
                if (i == (int)parent->children.size - 2)
                {
                    parent->children.data[i + 1]->flag = 4;
                }
                else
                {
                    parent->children.data[i + 1]->flag = 1;
                }
            }
        }
    }
    return true;
}

static void JSON(FILE *file, XMLNode *node, int indent, int times)
{
    for (int i = 0; i < node->children.size; i++)
    {
        XMLNode *child = node->children.data[i];
        if (times > 0)
        {
            fprintf(file, "%0*s", indent * times, " ");
        }
        if (child->children.size == 0)
        {
            if (child->flag == 2)
            {
                fprintf(file, "\"%s\":[\n", child->tag);
                fprintf(file, "%0*s", indent * times, " ");
                fprintf(file, "{\n");
            }
            if (child->inner_text_size > 200) // if inner text size is large make new line
            {
                if (child->flag != 1 && child->flag != 4)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
                fprintf(file, "\n%0*s\"%s\",\n%0*s\n", (indent * (times + 1)), " ", child->inner_text, (indent * (times)), " ");
            }
            else // same line if inner text small
            {
                if (child->flag != 1 && child->flag != 4 && child->flag != 2)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
                fprintf(file, "\"%s\",\n", child->inner_text);
            }
            if (child->flag == 4)
            {
                fprintf(file, "]\n");
            }
        }
        else
        {
            if (are_same_children(child) && child->children.size >= 1)
            {
                child->flag = 3;
                fprintf(file, "\"%s\" :{\n", child->tag);
            }
            else
            {
                // if (child->children.size == 1)
                // {
                //     fprintf(file, "\"%s\" :{\n", child->tag);
                // }
                if (child->flag == 2)
                {
                    fprintf(file, "\"%s\":[\n", child->tag);
                    fprintf(file, "%0*s", indent * times, " ");
                    fprintf(file, "{\n");
                }
                else if (child->flag == 3)
                {
                }
                else if (child->flag != 1 && child->flag != 4)
                {
                    fprintf(file, "\"%s\": ", child->tag);
                }
            }
            fprintf(file, "\n");
        }
    }
}
