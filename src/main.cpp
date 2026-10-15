#include "math.hpp"
#include "graph.hpp"
#include<iostream>

int main()
{
    std::cout<<add(3,5)<<std::endl;

    Graph g(100);

    g.addEdge(1,2,100);

    return 0;
}
