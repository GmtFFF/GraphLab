#include "graph.hpp"
#include<vector>


Graph::Graph(int n)
{
    this->n=n;
    adj.resize(n);
}

void Graph::addEdge(int u,int v,int w)
{
    adj[u].push_back({v,w});
}

const std::vector<Edge>& Graph::neighbors(int u) const
{
    return adj[u];
}

int Graph::size() const
{
    return n;
}
