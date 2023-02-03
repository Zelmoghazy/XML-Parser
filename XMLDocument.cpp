#include "XMLDocument.h"

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
bool XMLDocument::checkBalance(std::string source, int n)
{
    int opening = 0;
    int closing = 0;
    std::stack<int> s;
    for (int i = 0; i < n; i++)
    {
        if (source[i] == '<')
        {
            if(source[i+1]== '?' || source[i+1] == '!'){
                i+=2;
                while(source[i] != '>'){
                    i++;
                }
            }
            else
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
