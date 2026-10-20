#include "tarjan.hpp"
#include "graph.hpp"
#include<vector>
#include<algorithm>


SCC::SCC(int n)
{
    this->n=n;
    tim=head=cnt=0;
    dfn.resize(n);
    low.resize(n);
    inst.resize(n);
    st.resize(n+2);
    color.resize(n);
    for(int i=0;i<=n+1;i++)st[i]=-1;
}

void SCC::Tarjan(const Graph &g,int x)
{
    dfn[x]=++tim;
    low[x]=dfn[x];
    inst[x]=1;
    st[++head]=x;
    for(const auto &i:g.neighbors(x))
    {
        if(!dfn[i.to])
        {
            Tarjan(g,i.to);
            low[x]=std::min(low[x],low[i.to]);
        }
        else if(inst[i.to])low[x]=std::min(low[x],dfn[i.to]);
    }
    if(dfn[x]==low[x])
    {
        cnt++;
        while(st[head+1]!=x)
        {
            color[st[head]]=cnt;
            inst[st[head]]=0;
            head--;
        }
    }
}

int SCC::count() const
{
    return cnt;
}

int SCC::belong(int x) const
{
    return color[x];
}
