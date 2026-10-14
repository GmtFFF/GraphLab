#ifndef GRAPH_HPP
#define GRAPH_HPP
#include<vector>

struct Edge
{
    int to,weight;
};

class Graph
{
    private:

        int n;
        std::vector<std::vector<Edge>>adj;

    public:

        Graph(int n);

        void addEdge(int u,int v,int w);

        const std::vector<Edge>& neighbors(int u) const;


};
#endif