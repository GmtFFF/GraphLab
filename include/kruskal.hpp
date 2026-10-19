#ifndef KRUSKAL_HPP
#define KRUSKAL_HPP

#include<vector>
#include<optional>

struct WeightEdge
{
    int u,v,w;
    bool operator <(const WeightEdge &x)const
    {
        return w<x.w;
    }
};

std::optional<long long> kruskal(int n,std::vector<WeightEdge>edges);

#endif