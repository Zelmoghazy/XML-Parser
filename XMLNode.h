#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <algorithm>


class XMLNode{
private:
    char *tag;
    char *inner_text;
    int inner_text_size;
    int flag;
    XMLNode *parent;
    std::vector<XMLNode *> children;
public:
    XMLNode();
    XMLNode(XMLNode *parent);
    ~XMLNode();
    XMLNode *XMLNode_at(int index);
    friend class XMLDocument;
};
