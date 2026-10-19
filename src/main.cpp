#include "graph.hpp"
#include "dijkstra.hpp"
#include "dsu.hpp"
#include "kruskal.hpp"
#include <iostream>
#include <optional>
#include <random>
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

    auto checkDSU = [&](const char* name, bool passed)
    {
        ++total;
        if (!passed) ++failed;
        std::cout << (passed ? "[PASS] DSU: " : "[FAIL] DSU: ") << name << '\n';
    };

    std::cout << "\nDSU tests\n";

    DSU singleSet(1);
    checkDSU("Single node is its own root", singleSet.find(0) == 0);
    singleSet.unite(0, 0);
    checkDSU("Self union preserves a singleton", singleSet.same(0, 0)
             && singleSet.find(0) == 0);

    DSU sets(6);
    bool initiallySeparate = true;
    for (int i = 0; i < 6; ++i)
    {
        initiallySeparate &= sets.find(i) == i && sets.same(i, i);
        for (int j = i + 1; j < 6; ++j)
            initiallySeparate &= !sets.same(i, j);
    }
    checkDSU("Nodes start in separate components", initiallySeparate);

    sets.unite(0, 1);
    checkDSU("Union connects both directions", sets.same(0, 1) && sets.same(1, 0));
    checkDSU("Other nodes remain separate", !sets.same(0, 2) && !sets.same(1, 5));

    sets.unite(1, 2);
    // Check before calling find(): it must not need external path compression.
    checkDSU("Connectivity is transitive", sets.same(0, 2) && sets.same(2, 0));
    const int root = sets.find(0);
    checkDSU("Connected nodes have the same root", root == sets.find(1)
             && root == sets.find(2));

    sets.unite(2, 3);
    sets.unite(3, 4);
    checkDSU("Longer chains stay connected", sets.same(0, 4)
             && sets.same(1, 3) && !sets.same(0, 5));

    sets.unite(0, 4);
    sets.unite(4, 0);
    sets.unite(5, 5);
    bool repeatedUnionsCorrect = sets.find(5) == 5;
    for (int i = 0; i < 5; ++i)
    {
        repeatedUnionsCorrect &= sets.same(i, 0) && !sets.same(i, 5);
        for (int j = 0; j < 5; ++j)
            repeatedUnionsCorrect &= sets.same(i, j);
    }
    checkDSU("Repeated and self unions preserve components", repeatedUnionsCorrect);

    // Compare against an independent component-label model with a fixed seed.
    const int nodeCount = 16;
    DSU randomSets(nodeCount);
    std::vector<int> component(nodeCount);
    for (int i = 0; i < nodeCount; ++i) component[i] = i;
    std::mt19937 random(20261019);
    bool matchesReference = true;
    for (int step = 0; step < 100 && matchesReference; ++step)
    {
        const int x = static_cast<int>(random() % nodeCount);
        const int y = static_cast<int>(random() % nodeCount);
        randomSets.unite(x, y);
        const int oldComponent = component[x];
        const int newComponent = component[y];
        for (int& label : component)
            if (label == oldComponent) label = newComponent;

        for (int i = 0; i < nodeCount && matchesReference; ++i)
        {
            for (int j = 0; j < nodeCount; ++j)
            {
                const bool expected = component[i] == component[j];
                const bool actual = randomSets.same(i, j);
                if (actual != expected)
                {
                    std::cout << "  After union " << step + 1 << " (" << x
                              << ", " << y << "): same(" << i << ", " << j
                              << ") expected " << expected << ", got " << actual << '\n';
                    matchesReference = false;
                    break;
                }
            }
        }
    }
    checkDSU("Fixed-seed unions match reference model", matchesReference);

    auto printMSTResult = [](const std::optional<long long>& result)
    {
        if (result.has_value()) std::cout << *result;
        else std::cout << "nullopt (no spanning tree)";
    };

    auto checkKruskal = [&](const char* name, int n,
                           const std::vector<WeightEdge>& edges,
                           std::optional<long long> expected)
    {
        ++total;
        const auto actual = kruskal(n, edges);
        const bool passed = actual == expected;
        if (!passed) ++failed;
        std::cout << (passed ? "[PASS] Kruskal: " : "[FAIL] Kruskal: ") << name << '\n';
        std::cout << "  Expected: ";
        printMSTResult(expected);
        std::cout << ", actual: ";
        printMSTResult(actual);
        std::cout << '\n';
    };

    std::cout << "\nKruskal tests\n";

    checkKruskal("Single node", 1, {}, 0);
    checkKruskal("Graph without edges has no spanning tree", 4, {}, std::nullopt);

    checkKruskal("Single node with self-loops has zero total weight", 1,
                 {{0, 0, -10}, {0, 0, 5}}, 0);

    checkKruskal("Input already forms a tree", 4,
                 {{0, 1, 8}, {1, 2, -3}, {2, 3, 4}}, 9);

    // Edges arrive unsorted; the cheapest spanning tree weighs 1 + 2 + 3.
    checkKruskal("Unsorted connected graph", 4,
                 {{0, 3, 10}, {0, 2, 4}, {1, 3, 5},
                  {2, 3, 1}, {0, 1, 3}, {1, 2, 2}}, 6);

    checkKruskal("Cycle edge is skipped", 3,
                 {{0, 1, 4}, {1, 2, 2}, {2, 0, 1}}, 3);

    checkKruskal("Equal weights allow multiple minimum trees", 4,
                 {{0, 1, 7}, {1, 2, 7}, {2, 3, 7},
                  {3, 0, 7}, {0, 2, 7}}, 21);

    // Reverse endpoints still describe the same undirected connection.
    checkKruskal("Parallel and reversed edges", 3,
                 {{0, 1, 9}, {1, 0, 2}, {1, 2, 3},
                  {0, 2, 8}, {0, 1, 2}}, 5);

    checkKruskal("Negative weights", 4,
                 {{0, 1, -4}, {1, 2, -2}, {2, 3, 3},
                  {0, 3, 10}, {0, 2, 0}, {1, 3, 5}}, -3);

    // A valid negative weight must not be mistaken for a failure sentinel.
    checkKruskal("Minus one is a valid spanning tree weight", 2,
                 {{0, 1, -1}}, -1);

    checkKruskal("Zero-weight spanning tree", 4,
                 {{0, 1, 0}, {1, 2, 0}, {2, 3, 0}, {0, 3, 1}}, 0);

    checkKruskal("Negative self-loops are ignored", 3,
                 {{0, 0, -100}, {1, 1, -50}, {0, 1, 4},
                  {1, 2, 2}, {0, 2, 9}}, 6);

    // A forest weight is not a valid MST result under the optional-return API.
    checkKruskal("Disconnected graph and isolated node", 6,
                 {{0, 1, 2}, {1, 2, 1}, {0, 2, 8}, {3, 4, -2}}, std::nullopt);

    // Having at least n-1 input edges does not imply the graph is connected.
    checkKruskal("Many edges but still disconnected", 4,
                 {{0, 1, 1}, {1, 2, 2}, {2, 0, 3},
                  {0, 1, 4}, {3, 3, -10}}, std::nullopt);

    // Each weight fits int, but their sum requires a 64-bit accumulator.
    checkKruskal("Total weight exceeds int range", 3,
                 {{0, 1, 1500000000}, {1, 2, 1500000000},
                  {0, 2, 2000000000}}, 3000000000LL);

    std::cout << '\n' << total - failed << '/' << total << " tests passed.\n";
    return failed == 0 ? 0 : 1;
}
