cmake_minimum_required(VERSION 3.10)

project(GraphLab)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(
    GraphLab
    src/main.cpp
    src/math.cpp
    src/graph.cpp
)