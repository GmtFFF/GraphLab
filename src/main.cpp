#include "graph.hpp"
#include "dijkstra.hpp"
#include <iostream>
#include <vector>

int main()
{
    // Keep this consistent with the unreachable-distance value in dijkstra.cpp.
    const int INF = 0x3f3f3f3f;
    int total = 0;
    int failed = 0;

    auto printDistances = [INF](const std::vector<int>& distances)
    {
        for (int distance : distances)
        {
            if (distance == INF) std::cout << "INF ";
            else std::cout << distance << ' ';
        }
        std::cout << '\n';
    };

    auto check = [&](const char* name, const Graph& graph, int source,
                     const std::vector<int>& expected)
    {
        ++total;
        const auto actual = dijkstra(graph, source);
        const bool passed = actual == expected;
        if (!passed) ++failed;

        std::cout << (passed ? "[PASS] " : "[FAIL] ") << name << '\n';
        std::cout << "  Actual:   ";
        printDistances(actual);
        if (!passed)
        {
            std::cout << "  Expected: ";
            printDistances(expected);
        }
    };

    // A shorter indirect path must replace the initial direct path.
    Graph graph(5);
    graph.addEdge(0, 1, 10);
    graph.addEdge(0, 2, 3);
    graph.addEdge(2, 1, 2);
    graph.addEdge(1, 3, 4);
    graph.addEdge(2, 3, 8);
    graph.addEdge(3, 4, 1);
    check("Shortest paths", graph, 0, {0, 5, 3, 9, 10});

    // Edges are directed; node 0 cannot be reached from node 1.
    Graph disconnected(4);
    disconnected.addEdge(0, 1, 7);
    disconnected.addEdge(1, 2, 2);
    check("Nonzero source and unreachable nodes", disconnected, 1,
          {INF, 0, 2, INF});

    // Parallel edges and a zero-weight cycle must still give correct distances.
    Graph zeroWeight(4);
    zeroWeight.addEdge(0, 1, 9);
    zeroWeight.addEdge(0, 1, 0);
    zeroWeight.addEdge(1, 0, 0);
    zeroWeight.addEdge(1, 2, 2);
    zeroWeight.addEdge(0, 2, 10);
    zeroWeight.addEdge(2, 3, 0);
    check("Zero weights, parallel edges and cycle", zeroWeight, 0, {0, 0, 2, 2});

    Graph single(1);
    check("Single node", single, 0, {0});

    Graph noEdges(3);
    check("Graph without edges", noEdges, 2, {INF, INF, 0});

    std::cout << '\n' << total - failed << '/' << total << " tests passed.\n";
    return failed == 0 ? 0 : 1;
}
