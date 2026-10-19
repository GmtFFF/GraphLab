#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include<vector>

struct WeightEdge
{
    int u,v,w;
    bool operator <(WeightEdge x)
    {
        return w<x.w;
    }
};

long long kruskal(int n,std::vector<WeightEdge>edges);

#endif