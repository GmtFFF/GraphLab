#include "dijkstra.hpp"
#include "graph.hpp"
#include<vector>
#include<queue>


std::vector<int> dijkstra(const Graph &g,int u)
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
    std::vector<int>dis(g.size(),INF);
    dis[u]=0;
    q.push({u,0});
    while(!q.empty())
    {
        int x=q.top().u,w=q.top().w;
        q.pop();
        if(w!=dis[x])continue;
        for(const auto &i:g.neighbors(x))
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