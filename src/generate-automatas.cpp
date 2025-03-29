#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include "types/types.h"

Automata make_labeled_copy(const Graph& original, unsigned int mask) {
    Automata labeled;
    std::map<Graph::vertex_descriptor, Automata::vertex_descriptor> vertex_map;

    // Копируем вершины
    for (auto [vi, vi_end] = boost::vertices(original); vi != vi_end; ++vi) {
        auto v_new = boost::add_vertex(labeled);
        vertex_map[*vi] = v_new;
    }

    for (auto [ei, ei_end] = boost::edges(original); ei != ei_end; ++ei) {
        auto src = boost::source(*ei, original);
        auto tgt = boost::target(*ei, original);

        auto new_edge = boost::add_edge(vertex_map[src], vertex_map[tgt], labeled).first;
        labeled[new_edge].marker = (mask >> src) & 1;
    }

    return labeled;
}

int main() {
    std::string input;
    std::cout << std::left << "Enter graph path: ";
    std::getline(std::cin, input);

    std::filesystem::path path(input);

    auto reader = SingleGraphReader();
    auto g = reader.readGraph(path);
    for (int i = 0; i < 1 << g.m_vertices.capacity(); i++) {
        ConsoleRecorder<Automata>().recordGraph(make_labeled_copy(g, i));
    }
}