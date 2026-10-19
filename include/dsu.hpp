#ifndef DSU_HPP
#define DSU_HPP
#include<vector>


class DSU
{
    private:
        
        std::vector<int>fa,siz;

    public:

        DSU(int n);
        int find(int x);
        bool unite(int x,int y);
        bool same(int x,int y);


};




#endif