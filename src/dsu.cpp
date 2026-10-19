#include "dsu.hpp"
#include<vector>

DSU::DSU(int n)
{
    fa.resize(n);
    siz.resize(n);
    for(int i=0;i<n;i++)fa[i]=i,siz[i]=1;
}

int DSU::find(int x)
{
    if(x==this->fa[x])return x;
    return this->fa[x]=find(this->fa[x]);
}

bool DSU::unite(int x,int y)
{
    bool s=same(x,y);
    if(!s)
    {
        this->fa[find(x)]=find(y);
        siz[y]+=siz[x];
    }
    return !s;
}

bool DSU::same(int x,int y)
{
    return find(x)==find(y);
}