#include "ConsoleRecorder.h"
#include <boost/graph/graphviz.hpp>
#include <types/types.h>


template<>
void ConsoleRecorder<DirectedGraph>::recordGraph(DirectedGraph& g, std::string name) {
    std::cout << "Graph_" << name << ":" << '\n';
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
void ConsoleRecorder<BisetGraph>::recordGraph(BisetGraph& g, std::string name) {
    std::cout << "Graph_" << name << ":" << '\n';
    for (int v = 0; v < g.m_vertices.size(); ++v) {
        std::cout << v << ": ";
        auto edges = out_edges(v, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            std::cout << target_vertex << "[" << g[*it].meta << "]" << " ";
        }
        std::cout << '\n';
    }
    std::cout << "------------------------" << '\n';
}

template<>
void ConsoleRecorder<UndirectedGraph>::recordGraph(UndirectedGraph& g, std::string name) {
    std::cout << "Graph_" << name << ":" << '\n';
    auto [vi, vi_end] = boost::vertices(g);
    for (auto it = vi; it != vi_end; ++it) {
        std::cout << g[*it].node_id << "[" << g[*it].fillcolor << "]" << ": ";
        auto edges = out_edges(*it, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            std::cout << target_vertex << " ";
        }
        std::cout << '\n';
    }
    std::cout << "------------------------" << '\n';
}

template<>
void ConsoleRecorder<Automata>::recordGraph(Automata& g, std::string name) {
    std::cout << "Graph_" << name << ":" << '\n';
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

template class ConsoleRecorder<DirectedGraph>;
template class ConsoleRecorder<Automata>;
template class ConsoleRecorder<UndirectedGraph>;
template class ConsoleRecorder<BisetGraph>;