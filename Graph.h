#include "XMLNode.h"

class Graph{
private:
    int NumVertices;
    bool **edges;
public:
    Graph(int NumVertices);
    ~Graph();
    bool has_edge(unsigned int from_node, unsigned int to_node);
    bool add_edge(unsigned int from_node, unsigned int to_node);
    bool Dotwrite(const char *path);
    int count_edge(unsigned int to_node);
    void most_active();
    std::vector<int> are_mutual(unsigned int u1, unsigned int u2);
    void mutual_followers(unsigned int u1, unsigned int u2);
    std::vector<int> are_not_mutual(unsigned int u1);
    void suggested_followers(unsigned int user);
    friend class XMLDocument;
};
