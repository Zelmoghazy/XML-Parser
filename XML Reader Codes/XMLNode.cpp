#include "XMLNode.h"
XMLNode::XMLNode(){}

XMLNode::XMLNode(XMLNode *parent)
{
    this->parent = parent;
    this->tag = NULL;
    this->inner_text = NULL;
    this->inner_text_size = 0;
    this->flag = 0;
    if (parent)
    {
        parent->children.push_back(this);
    }
};

XMLNode::~XMLNode()
{
    free(this->tag);
    free(this->inner_text);
    free(this);
}

XMLNode* XMLNode::XMLNode_at(int index)
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
