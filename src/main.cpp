#include "math.hpp"
#include "graph.hpp"
#include<iostream>

int main()
{
    std::cout<<add(3,5)<<std::endl;

    Graph g(5);
    g.addEdge(0,1,10);
    g.addEdge(0,2,3);
    g.addEdge(2,1,2);
    g.addEdge(1,3,4);
    g.addEdge(2,3,8);
    g.addEdge(3,4,1);

    std::vector<int> dis=g.dijkstra(0);
    for(int i=0;i<5;i++)
    std::cout<<dis[i]<<" ";

    return 0;
}
