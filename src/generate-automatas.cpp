#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>

#include "types/types.h"

Automata make_automata_from_graph(const Graph& original, unsigned int mask) {
    Automata labeled;
    std::map<Graph::vertex_descriptor, Automata::vertex_descriptor> vertex_map;

    // vertexes copy
    for (auto [vi, vi_end] = boost::vertices(original); vi != vi_end; ++vi) {
        auto v_new = boost::add_vertex(labeled);
        vertex_map[*vi] = v_new;
    }

    unsigned int marked = 0;
    for (auto [ei, ei_end] = boost::edges(original); ei != ei_end; ++ei) {
        auto src = boost::source(*ei, original);
        auto tgt = boost::target(*ei, original);

        auto new_edge = boost::add_edge(vertex_map[src], vertex_map[tgt], labeled).first;
        labeled[new_edge].mark = (mask >> src ^ marked >> src) & 1 ? 'b' : 'a';
        marked |= 1 << src;
    }

    auto [vi, vi_end] = boost::vertices(labeled);
    for (auto it = vi; it != vi_end; ++it) {
        labeled[*it].node_id = *it;
    }

    return labeled;
}

int main() {
    auto reader = SingleGraphReader<Graph>("./graphs/4/0.dot");
    // auto recorder = ConsoleRecorder<Automata>();
    auto recorder = DiskRecorder<Automata>("./automatas/");
    auto g = reader.readGraph();

    for (int i = 0; i < 1 << g.m_vertices.size(); i++) {
        recorder.recordGraph(make_automata_from_graph(g, i));
    }

    DiskRecorder<Graph>(recorder.dirPath, "./", false)
    .setFilename("graph.dot")
    .recordGraph(g);

    auto reader2 = SingleGraphReader<Automata>("./automatas/4/0.dot");
    auto automata = reader2.readGraph();

    DiskRecorder<Automata>(recorder.dirPath, "./", false)
    .setFilename("new_graph.dot")
    .recordGraph(automata);
}