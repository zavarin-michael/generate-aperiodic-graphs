#include "AutomatasFromGraph.h"

template<>
AutomatasFromGraph<AutomataGenerationResult>::AutomatasFromGraph(DirectedGraph& g) : graph(g) {}

template<>
AutomataPairCoroutine::pull_type AutomatasFromGraph<AutomataGenerationResult>::generateGraphs() {
    return AutomataPairCoroutine::pull_type([this](AutomataPairCoroutine::push_type &yield) {
        for (int mask = 0; mask < 1 << graph.m_vertices.size(); mask++) {
            auto copy = std::make_shared<Automata>();
            auto &labeled = *copy;

            std::map<DirectedGraph::vertex_descriptor, Automata::vertex_descriptor> vertex_map;

            // vertexes copy
            for (auto [vi, vi_end] = boost::vertices(graph); vi != vi_end; ++vi) {
                auto v_new = boost::add_vertex(labeled);
                vertex_map[*vi] = v_new;
            }

            unsigned int marked = 0;
            for (auto [ei, ei_end] = boost::edges(graph); ei != ei_end; ++ei) {
                auto src = boost::source(*ei, graph);
                auto tgt = boost::target(*ei, graph);

                auto new_edge = boost::add_edge(vertex_map[src], vertex_map[tgt], labeled).first;
                labeled[new_edge].mark = (mask >> src ^ marked >> src) & 1 ? 'b' : 'a';
                marked |= 1 << src;
            }

            auto [vi, vi_end] = boost::vertices(labeled);
            for (auto it = vi; it != vi_end; ++it) {
                labeled[*it].node_id = *it;
            }

            AutomataGenerationResult result;
            result.mask = mask;
            result.automata = copy;

            yield(result);
        }
    });
}

template class AutomatasFromGraph<AutomataGenerationResult>;


