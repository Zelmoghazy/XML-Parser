#include "XMLNode.h"
#include "Graph.h"

class XMLDocument
{
private:
    XMLNode *root;
public:
    XMLDocument(/* args */);
    ~XMLDocument();
    XMLNode* get_users();
    XMLNode* get_user(unsigned int);
    bool checkBalance(std::string, int);
    bool XMLDocument_parse(const char *);
    void Format(FILE *, XMLNode *, int, int );
    void Minify(FILE *, XMLNode *);
    bool XMLDocument_write(const char *, const char);
    void print_ID(XMLNode *);
    void print_Name(XMLNode *);
    void print_Posts(XMLNode *);
    void print_Followers(XMLNode *);
    char *get_Follower_id(XMLNode *, int);
    XMLNode* getUser_ID(XMLNode *, const char *);
    XMLNode* getUser_Name(XMLNode *, const char *);
    std::vector<XMLNode*> get_post(const char *);
    void search_post(const char *);
    XMLNode* getFamousNode(XMLNode*);
    void printFamousNode(XMLNode* );
    Graph constructGraph(XMLNode* );
    bool are_same_children(XMLNode *);
    void JSON(FILE *, XMLNode *, int,int);
    Graph constructGraph(XMLNode* );
};
