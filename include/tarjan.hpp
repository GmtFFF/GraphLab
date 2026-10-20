#ifndef TARJAN_HPP
#define TARJAN_HPP
#include<vector>
#include "graph.hpp"

class SCC
{
    private:

        int n,tim,head,cnt;

        std::vector<int>dfn,low,st,color;

        std::vector<bool>inst;


    public:

        SCC(int n);

        void Tarjan(const Graph &g,int x);

        int count() const;

        int belong(int x) const;

};





#endif