#include <filesystem>
#include <Filters/Predicates/Predicates.h>
#include <Filters/SimpleFilter/SimpleFilter.h>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Generators/Functions/Functions.h>

#include "types/types.h"

void generateAutomatasGraph(Graph& g) {
    auto n = g.m_vertices.size();

    Graph new_graph;

    auto recorder = DiskRecorder<Graph>("./automatas-graphs/");
    auto cr = ConsoleRecorder<Graph>();
    cr.recordGraph(g);


    auto filter = SimpleFilter(std::vector<std::function<bool(Automata)>> {
        isSynchronized,
    });

    std::map<int, Graph::vertex_descriptor> vertex_map;

    for (int mask = 0; mask < 1 << g.m_vertices.size(); mask++) {
        auto v_new = boost::add_vertex(new_graph);
        vertex_map[mask] = v_new;
    }

    auto [vi, vi_end] = boost::vertices(new_graph);
    for (auto it = vi; it != vi_end; ++it) {
        new_graph[*it].node_id = *it;
    }

    size_t count = 0;
    for (auto [mask, automata_ptr]  : makeAutomatasFromGraph(g)) {
        for (int i = 0; i < n; ++i) {
            unsigned int flipped = mask ^ (1 << i);
            boost::add_edge(vertex_map[mask], vertex_map[flipped], new_graph);
        }
        auto& automata = *automata_ptr;
        if (filter.isAccepted(automata)) {
            new_graph[vertex_map[mask]].fillcolor = "green";
            count++;
        }
    }

    recorder.recordGraph(new_graph);
    recorder.setFilename("graph.dot").recordGraph(g);

    std::cout << count << std::endl;
}

int main() {
    // auto reader = SingleGraphReader<Automata>("./automatas/4/0.dot");
    auto reader = SingleGraphReader<Graph>();
    // auto recorder = ConsoleRecorder<Automata>();
    // auto recorder = DiskRecorder<Graph>("./analyzes/");
    auto graph = reader.readGraph();

    generateAutomatasGraph(graph);
}