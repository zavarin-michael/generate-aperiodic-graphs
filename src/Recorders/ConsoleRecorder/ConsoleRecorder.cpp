#include "ConsoleRecorder.h"
#include <filesystem>
#include <boost/graph/graphviz.hpp>

void ConsoleRecorder::recordGraph(const Graph& g) {
    std::cout << "Graph:" << '\n';
    for (int v = 0; v < g.m_vertices.capacity(); ++v) {
        std::cout << v << ": ";
        auto edges = out_edges(v, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            std::cout << target_vertex << " ";
        }
        std::cout << '\n';
    }
    std::cout << "------------------------" << '\n';
}
