#include "ConsoleRecorder.h"
#include <boost/graph/graphviz.hpp>
#include <types/types.h>


template<>
void ConsoleRecorder<Graph>::recordGraph(const Graph& g) {
    std::cout << "Graph:" << '\n';
    for (int v = 0; v < g.m_vertices.size(); ++v) {
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

template<>
void ConsoleRecorder<Automata>::recordGraph(const Automata& g) {
    std::cout << "Graph:" << '\n';
    for (int v = 0; v < g.m_vertices.size(); ++v) {
        std::cout << v << ": ";
        auto edges = out_edges(v, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            std::cout << target_vertex << "[" << g[*it].mark << "]" << " ";
        }
        std::cout << '\n';
    }
    std::cout << "------------------------" << '\n';
}

template class ConsoleRecorder<Graph>;
template class ConsoleRecorder<Automata>;