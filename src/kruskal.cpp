#include "dsu.hpp"
#include "kruskal.hpp"
#include<vector>
#include<algorithm>
#include<optional>


std::optional<long long> kruskal(int n,std::vector<WeightEdge>edge)
{
    std::sort(edge.begin(),edge.end());
    DSU dsu(n);
    long long sum=0,num=0;
    for(const auto &i:edge)
    {
        if(dsu.unite(i.u,i.v))
        {
            sum+=i.w;
            num++;
        }
    }
    if(num!=n-1)return std::nullopt;
    return sum;
}