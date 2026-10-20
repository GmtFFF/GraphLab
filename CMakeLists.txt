cmake_minimum_required(VERSION 3.10)

project(GraphLab)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(
    GraphLab
    src/main.cpp
    src/math.cpp
    src/graph.cpp
    src/dijkstra.cpp
    src/dsu.cpp
    src/kruskal.cpp
)

enable_testing()

add_executable(test_dsu
    tests/test_dsu.cpp
    src/dsu.cpp
)

target_include_directories(test_dsu PRIVATE include)

add_test(NAME DSU COMMAND test_dsu)


add_executable(test_kruskal
    tests/test_kruskal.cpp
    src/kruskal.cpp
    src/dsu.cpp
)

target_include_directories(test_kruskal PRIVATE include)

add_test(NAME Kruskal COMMAND test_kruskal)