#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <conio.h>

typedef struct Stack
{
    int top;
    int MAX;
    char *A;
} Stack;

/*Stack*/
void InitStack(Stack *S, int initialSize);
void FreeStack(Stack *S);
void Push(Stack *S, char value);
void Pop(Stack *S);
bool IsStackEmpty(Stack *S);
char stackTop(Stack *S);
bool checkBalance(const char *source, int n);

// dynamic array for pointers to children
typedef struct XMLNodeList
{
    int MAX;               // dynamically allocated size that is expanded when exceeded
    int size;              // keeps track of  number of children
    struct XMLNode **data; // pointer to pointer to node (List of pointer to nodes)
} XMLNodeList;

/*Tree Structure of XML Nodes*/
typedef struct XMLNode
{
    //<tag>inner_text</tag>
    char *tag;        // String to hold tag
    char *inner_text; // String to hold inner text
    int inner_text_size;
    int flag;
    struct XMLNode *parent;      // pointer to parent Node
    struct XMLNodeList children; // List of children nodes
} XMLNode;

void XMLNodeList_init(XMLNodeList *list); // Constructor
void XMLNodeList_add(XMLNodeList *list, XMLNode *node);
XMLNode *XMLNodeList_at(XMLNodeList *list, int index);
void XMLNodeList_free(XMLNodeList *list); // Destructor

XMLNode *XMLNode_new(XMLNode *parent);
void XMLNode_free(XMLNode *node);
XMLNode *XMLNode_child(XMLNode *parent, int index);
XMLNodeList *XMLNode_children(XMLNode *parent, const char *tag);

typedef struct XMLDocument
{
    XMLNode *root; // pointer to root node
} XMLDocument;

static void Format(FILE *file, XMLNode *node, int indent, int times);
static void Minify(FILE *file, XMLNode *node);
bool XMLDocument_load(XMLDocument *doc, const char *path);
bool XMLDocument_write(XMLDocument *doc, const char *path, const char choice);
void XMLDocument_free(XMLDocument *doc);
void print_ID(XMLNode *user);
void print_Name(XMLNode *user);
void print_Posts(XMLNode *user);
void print_Followers(XMLNode *user);
char *get_Follower_id(XMLNode *user, int index);
XMLNode *getUser_ID(XMLNode *users, const char *ID);
XMLNode *getUser_Name(XMLNode *users, const char *Name);
