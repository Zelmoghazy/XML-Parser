#include "XMLNode.h"

class Graph{
private:
    int NumVertices;
    bool **edges;
public:
    //constructor
    Graph(int NumVertices);
    //distractor
    ~Graph();
    //check if edge exits, edge[from][to] == 1
    bool has_edge(unsigned int from_node, unsigned int to_node);
    //add edge
    bool add_edge(unsigned int from_node, unsigned int to_node);
    //used for drawing the graph
    bool Dotwrite(const char *path);
    //count the number of edges of the "to_node"
    int count_edge(unsigned int to_node);
    //find the most active user
    void most_active();
    //returns a vector containing the mustual followers between two users 
    std::vector<int> are_mutual(unsigned int u1, unsigned int u2);
    //find mutual followers between two users
    void mutual_followers(unsigned int u1, unsigned int u2);
    //returns a vector of suggested users for sent user
    std::vector<int> are_not_mutual(unsigned int u1);
    //find suggestions for a user
    void suggested_followers(unsigned int user);
    friend class XMLDocument;
};
