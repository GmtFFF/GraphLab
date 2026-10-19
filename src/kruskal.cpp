#include "dsu.hpp"
#include "kruskal.hpp"
#include<vector>
#include<algorithm>


long long kruskal(int n,std::vector<WeightEdge>edge)
{
    sort(edge.begin(),edge.end());
    DSU dsu(n);
    long long sum=0;
    for(const auto &i:edge)
    {
        if(dsu.unite(i.u,i.v))sum+=i.w;
    }
    return sum;
}