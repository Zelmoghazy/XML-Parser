#include "Graph.h"

Graph::Graph(int NumVertices)
{
    this->NumVertices = NumVertices;
    this->edges = (bool**) calloc(sizeof(bool *), this->NumVertices);
    for (int i = 0; i < NumVertices; i++)
    {
        this->edges[i] = (bool*) calloc(sizeof(bool), this->NumVertices);
    }
}

Graph::~Graph()
{
    if (this->edges == NULL)
    {
        free(this);
        return;
    }
    for (int i = 0; i < this->NumVertices; i++)
    {
        if (this->edges[i] != NULL)
        {
            free(this->edges[i]);
        }
    }
    free(this->edges);
}

bool Graph::has_edge(unsigned int from_node, unsigned int to_node)
{
    return this->edges[from_node][to_node];
}

bool Graph::add_edge(unsigned int from_node, unsigned int to_node)
{
    if (has_edge(from_node, to_node))
    {
        return false;
    }
    this->edges[from_node][to_node] = true;
    return true;
}

bool Graph::Dotwrite(const char *path)
{
    FILE *file = fopen(path, "w");
    if (!file)
    {
        fprintf(stderr, "Cannot open file to write");
        return false;
    }
    fprintf(file, "digraph {\n");
    for (int i = 0; i < this->NumVertices; i++)
    {
        for (int j = 0; j < this->NumVertices; j++)
        {
            if (this->edges[i][j])
            {
                fprintf(file, "%d -> %d;\n", i+1, j+1);
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    return true;
}

int Graph::count_edge(unsigned int to_node)
{
    int count = 0;
    for (int i = 0; i < this->NumVertices; i++)
    {
        for (int j = 0; j < this->NumVertices; j++)
        {
            if (this->edges[i][j] && j == to_node)
            {
                count++;
            }
        }
    }
    return count;
}

void Graph::most_active()
{
    int max = 0;
    int temp = 0;
    int id = 0;
    for(int i=0; i< this->NumVertices; i++)
    {
        temp = this->count_edge(i);
        if (max < temp)
        {
            max = temp;
            id = i;
        }
    }
    std::cout<<"The most active user is: "<<id+1<<std::endl;
}

std::vector<int> Graph::are_mutual(unsigned int u1, unsigned int u2)
{
    u1 = u1-1;
    u2 = u2-1;
    std::vector<int> result;
    for (int i = 0; i < this->NumVertices; i++)
    {
        if(this->edges[u1][i] && this->edges[u2][i])
            result.push_back(i+1);
    }
    return result;
}

void Graph::mutual_followers(unsigned int u1, unsigned int u2)
{
    std::vector<int> result = this->are_mutual(u1,u2);
    if(!result.empty())
    {
        std::cout<<"The mutal followers between users "<<u1<<" and "<<u2<<" are:"<<std::endl;
        for(int i=0; i<result.size(); i++)
            std::cout<<result[i]<<std::endl;
    }
    else
        std::cout<<"There are no mutual follwers between users "<<u1<<" and "<<u2<<std::endl;
    return;
}

std::vector<int> Graph::are_not_mutual(unsigned int u1)
{
    std::vector<int> result;
    std::vector<int>::iterator temp;
    for (int i = 0; i < this->NumVertices; i++)
    {
        for(int j = 0; j < this->NumVertices; j++)
        {
            if (this->edges[u1][i]){
                if(this->edges[i][j] && !this->edges[u1][j] && u1!= j)
                {
                    result.push_back(j+1);
                }
            }    
        }
    }
    temp = std::unique(result.begin(), result.end());
    result.resize(std::distance(result.begin(), temp));
    return result;
}

void Graph::suggested_followers(unsigned int user)
{
    user = user-1;
    std::vector<int> result = this->are_not_mutual(user);
    if(!result.empty())
    {
        std::cout<<"We suggest the following followers for you:"<<std::endl;
        for(int i=0; i<result.size(); i++)
            std::cout<<result[i]<<std::endl;
    }
    else
        std::cout<<"No suggestions available"<<std::endl;
    return;
}
