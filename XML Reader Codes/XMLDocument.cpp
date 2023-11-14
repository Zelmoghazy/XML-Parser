#include "XMLDocument.h"
#include "Graph.h"

XMLDocument::XMLDocument(/* args */)
{
    this->root = NULL;
}

XMLDocument::~XMLDocument()
{
    free(this->root);
}

XMLNode* XMLDocument::get_users()
{
    return this->root->children[0];
}

XMLNode* XMLDocument::get_user(unsigned int id)
{
    return this->root->children[0]->children[id-1];
}

bool XMLDocument::checkBalance(std::string source)
{
    int opening = 0;
    int closing = 0;
    std::stack<int> s;
    for (int i = 0; i < source.size(); i++)
    {
        if (source[i] == '<')
        {
            if(source[i+1]== '?' || source[i+1] == '!'){
                i+=2;
                while(source[i] != '>'){
                    i++;
                }
            }else{
                s.push('<');
                if (source[i + 1] == '/'){
                    closing++;
                }else{
                    opening++;
                }
            }
        }
        else if (source[i] == '>')
        {
            if (s.empty()){
                return false;
            }
            else if (s.top() == '<'){
                s.pop();
            }
        }
    }
    if (s.empty() && (closing - opening) == 0){
        return true;
    }
    return false;
}

bool XMLDocument::XMLDocument_parse(const char *path)
{
    FILE *file = fopen(path, "r");
    // Check if file loads properly
    if (!file){
        // output to stderr to seperate error messages from regular output
        fprintf(stderr, "Couldnt load file");
        return false;
    }
    std::string source;
    char ch;
    while (file)
    {
        ch = fgetc(file);
        if (ch == EOF)
            break;
        source.push_back(ch);
    }
    source.push_back('\0');
    fclose(file);
    /*-----------------------------------------------------*/
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
    if (checkBalance(source)){
        printf("the xml is balanced\n");
    }else{
        printf("The XML is not balanced\n");
        //return false;
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
            }else if (source[i + 1] == '?')
            {
                i++;
                char start = source[i];
                while (source[++i]!= '?');
                char end = source[i];
                if (start != end )
                {
                    std::cout<<"invalid header!!";
                    return false;
                }
                while (source[i]!= '<'){i++;}
            }
            // if we find a '/' after '<' we are at the end of node
            else if (source[i + 1] == '/')
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
        fprintf(stderr, "missing end tag/tags\n");
        if(buffer_index > 0){
            buffer[buffer_index] = '\0';
            // Append extracted inner text to our buffer and make note of size
            curr_node->inner_text_size += buffer_index;
            curr_node->inner_text = strdup(buffer);
        }
        while(curr_node->parent)
            curr_node = curr_node->parent;
        //return false;
    }
    this->root = curr_node;
    return true;
}

bool XMLDocument::are_same_children(XMLNode *parent)
{
    bool mark = false;
    int counter = 0;
    for (int i = 0; i < parent->children.size() - 1; i++)
    {
        if (strcmp(parent->children[i]->tag, parent->children[i + 1]->tag))
        {
            counter++;
            continue;
        }
        else
        {
            if (mark == false)
            {
                parent->children[i]->flag = 2;
                if (i == (int)parent->children.size() - 2)
                {
                    parent->children[i + 1]->flag = 4;
                }
                else
                {
                    parent->children[i + 1]->flag = 1;
                }
                mark = true;
            }
            else
            {
                parent->children[i]->flag = 1;
                if (i == (int)parent->children.size() - 2)
                {
                    parent->children[i + 1]->flag = 4;
                }
                else
                {
                    parent->children[i + 1]->flag = 1;
                }
            }
        }
    }
    if (counter == parent->children.size() - 1 && parent->children.size() != 1)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void XMLDocument::JSON(FILE *file, XMLNode *node, int indent, int times)
{

    for (int i = 0; i < node->children.size(); i++)
    {
        XMLNode *child = node->children[i];
        if (times > 0)
        {
            fprintf(file, "%0*s", indent * times, " ");
        }
        if (child->children.size() == 0)
        {
            if (child->flag == 2)
            {
                fprintf(file, "\"%s\":[\n", child->tag);
                fprintf(file, "%0*s", indent * times, " ");
            }

            if (child->flag != 1 && child->flag != 4 && child->flag != 2)
            {
                if (i == 0 && child->parent->flag != 3)
                {
                    fprintf(file, "{\n");
                    fprintf(file, "%0*s", indent * times, " ");
                }
                fprintf(file, "\"%s\": ", child->tag);
            }
            fprintf(file, "\"%s\"", child->inner_text);
            if (i != child->parent->children.size() - 1)
            {
                fprintf(file, ",\n");
            }
            else
            {
                fprintf(file, "\n");
            }

            if (child->flag == 4)
            {
                fprintf(file, "%0*s", indent * times + 1, " ");
                fprintf(file, "]\n");
            }
        }
        else
        {
            if (are_same_children(child) && child->flag == 0)
            {
                child->flag = 3;
                fprintf(file, "\"%s\" :{", child->tag);
            }
            else
            {
                if (child->flag == 2)
                {
                    fprintf(file, "\"%s\":[", child->tag);
                    fprintf(file, "%0*s", indent * times, " ");
                }
                else if (child->flag == 3)
                {
                }
                else if (child->flag != 1 && child->flag != 4)
                {
                    if (i == 0 && child->parent->flag != 3 && child->parent->flag != 0)
                    {
                        fprintf(file, "{\n");
                        fprintf(file, "%0*s", indent * times, " ");
                    }
                    fprintf(file, "\"%s\": ", child->tag);
                }
            }
            fprintf(file, "\n");
            JSON(file, child, indent, times + 1);
            if (times > 0)
            {
                fprintf(file, "%0*s", indent * times, " ");
            }

            if (i != child->parent->children.size() - 1)
            {
                fprintf(file, "},\n");
            }
            else
            {
                fprintf(file, "}\n");
            }
            if (child->flag == 4)
            {
                fprintf(file, "%0*s", indent * times - 2, " ");
                fprintf(file, "]\n");
            }
        }
    }
}

void XMLDocument::Format(FILE *file, XMLNode *node, int indent, int times)
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

void XMLDocument::Minify(FILE *file, XMLNode *node)
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

bool XMLDocument::XMLDocument_write(const char *path, const char choice)
{
    //open file for write
    //if the file is not exist create a new one
    FILE *file = fopen(path, "w");
    if (choice == 'f')
    {
        this->Format(file, this->root, 3, 0); // identation = 3 spaces (can be changed)
    }
    else if (choice == 'm')
    {
        Minify(file, this->root);
    }
    else if (choice == 'j')
    {
        fprintf(file, "{\n");
        JSON(file, this->root, 3, 1);
        fprintf(file,"}");
    }
    fclose(file);
    return true;
}

void XMLDocument::print_ID(XMLNode *user)
{
    XMLNode *id = user->children[0];
    printf("%s: %s\n", id->tag, id->inner_text);
}

void XMLDocument::print_Name(XMLNode *user)
{
    XMLNode *name = user->children[1];
    printf("%s: %s\n", name->tag, name->inner_text);
}

void XMLDocument::print_Posts(XMLNode *user)
{
    XMLNode *posts = user->children[2];
    int i = 1;
    for (XMLNode* post: posts->children)
    {
        printf("--------------------\n");
        printf("%s : %d\n", post->tag, i++);
        printf("--------------------\n");
        //print body
        XMLNode *body = post->children[0];
        printf("%s: %s\n", body->tag, body->inner_text);
        //print topics
        XMLNode *topics = post->children[1];
        for(XMLNode* topic : topics->children){
            printf("%s: %s\n", topic->tag, topic->inner_text);
        }
    }
}

void XMLDocument::print_Followers(XMLNode *user)
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

char* XMLDocument::get_Follower_id(XMLNode *user, int index)
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

XMLNode* XMLDocument::getUser_ID(XMLNode *users, const char *ID)
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

XMLNode* XMLDocument::getUser_Name(XMLNode *users, const char *Name)
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

std::vector<XMLNode*> XMLDocument::get_post(const char *word)
{
    std::vector<XMLNode*> result;
    for (XMLNode* user : this->root->children[0]->children)
    {
        XMLNode *posts = user->children[2];
        for (XMLNode* post : posts->children)
        {
            XMLNode *topics = post->children[1];
            for (XMLNode* topic : topics->children)
            {
                if (!strcmp(topic->inner_text, word))
                {
                    result.push_back(post);
                }
            }
            XMLNode *body = post->children[0];
            std::string temp = body->inner_text;
            size_t found = temp.find(word);
            if (found != std::string::npos)
            {
                result.push_back(post);
            }
        }
    }
    return result;
}

void XMLDocument::search_post(const char *word)
{
    std::vector<XMLNode*> result = this->get_post(word);
    if(result.empty())
    {
        std::cout<<"No post exist with such topic/keyword"<<std::endl;
        return;
    }
    else
    {
        std::cout<<"The posts with word/topic \""<<word<<"\" is/are:"<<std::endl;
        for(int i=0; i<result.size(); i++)
        {
            printf("--------------------\n");
            std::cout<<"post "<<i+1<<":"<<std::endl;
            //print body
            XMLNode *body = result[i]->children[0];
            printf("%s: %s\n", body->tag, body->inner_text);
            //print topics
            XMLNode *topics = result[i]->children[1];
            for(XMLNode* topic : topics->children){
                printf("%s: %s\n", topic->tag, topic->inner_text);
            }
        }
        return;
    }
}

XMLNode* XMLDocument::getFamousNode(XMLNode* root)
{
    if(!root)
        return root;
    int max = 0;
    XMLNode* maxNode = nullptr;
    for (XMLNode* child : root->children)
    {
        XMLNode* followers = child->children[3];
        if(!followers)
            continue;
        int num_followers = followers->children.size();
        if( num_followers > max){
            max = num_followers;
            maxNode = child;
        }
    }
    return maxNode;
}

void XMLDocument::printFamousNode(XMLNode* root)
{
    if(!root){
        std::cout<<"invalid pointer!!";
        return;
    }
    XMLNode* famous = getFamousNode(root);
    std::cout<<"the most famous user is: "<<famous->children[0]->inner_text<<std::endl;
}

Graph XMLDocument::constructGraph(XMLNode* root)
{
    //create a graph with n users
    Graph g(root->children.size());
    //add edges to the graph
    for (XMLNode* child : root->children)
    {
        int j = 0;
        //get index of followers
        for (XMLNode* children : child->children){
            char* tag = children->tag;
            if(tag[0] == 'f' || tag[0] == 'F')
                break;
            j++;
        }
        XMLNode* followers = child->children[j];
        if(!followers)
            continue;
        
        for (XMLNode* follower : followers->children)
        {
            XMLNode* id =  child->children[0];
            XMLNode* follower_id = follower->children[0];
            g.add_edge(atoi(id->inner_text) -1, atoi(follower_id->inner_text)-1);
        }
    }
    if (!g.Dotwrite("test.dot"))
    {
        printf("Error");
    }
    system("dot -Tpng -O test.dot");
    return g;
}
