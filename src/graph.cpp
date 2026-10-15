#include "graph.hpp"
#include<vector>
#include<queue>
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

std::vector<int> Graph::dijkstra(int u) const
{
    struct node
    {
        int u,w;
        bool operator <(const node &b) const
        {
            return w>b.w;
        }
    };
    std:: priority_queue<node>q;
    const int INF=0x3f3f3f3f;
    std::vector<int>dis(n,INF);
    dis[u]=0;
    q.push({u,0});
    while(!q.empty())
    {
        int x=q.top().u,w=q.top().w;
        q.pop();
        if(w!=dis[x])continue;
        for(const auto &i:neighbors(x))
        {
            if(dis[x]+i.weight<dis[i.to])
            {
                dis[i.to]=dis[x]+i.weight;
                q.push({i.to,dis[i.to]});
            }
        }
    }
    return dis;
}
