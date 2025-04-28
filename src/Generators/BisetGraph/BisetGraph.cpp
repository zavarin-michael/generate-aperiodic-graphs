#include "BisetGraph.h"

auto make_ordered_pair = [](auto a, auto b) {
    auto [min_v, max_v] = std::minmax(a, b);
    return std::make_pair(min_v, max_v);
};

template<>
BisetGraph<DirectedGraph>::BisetGraph(Automata& g) : automata(g) {}

template<>
GraphCoroutine::pull_type BisetGraph<DirectedGraph>::generateGraphs() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type &yield) {
        DirectedGraph g;

        std::vector<Automata::vertex_descriptor> automata_vertices;
        for (auto [vi, vi_end] = boost::vertices(automata); vi != vi_end; ++vi) {
            automata_vertices.push_back(*vi);
        }

        // mapping to source edges
        std::map<std::pair<int, int>, DirectedGraph::vertex_descriptor> pair_to_vertex;


        // all edges from source graph
        for (auto v1 : automata_vertices) {
            for (auto v2 : automata_vertices) {
                    auto p = make_ordered_pair(v1, v2);
                if (pair_to_vertex.contains(p))
                    continue;
                VertexProperties props;
                props.node_id = v1 * 100000 + v2;
                const auto vertex_in_g = boost::add_vertex(props, g);

                pair_to_vertex[p] = vertex_in_g;

                if (v1 == v2) {
                    g[vertex_in_g].fillcolor = "green";
                }
            }
        }

        for (auto v1 : automata_vertices) {
            for (auto v2 : automata_vertices) {
                std::map<char, std::vector<Automata::vertex_descriptor>> targets;

                // all transitions from v1
                for (auto [ei, ei_end] = boost::out_edges(v1, automata); ei != ei_end; ++ei) {
                    char label = automata[*ei].mark;
                    auto target = boost::target(*ei, automata);
                    targets[label].push_back(target);
                }

                // all transitions from v2
                for (auto [ei, ei_end] = boost::out_edges(v2, automata); ei != ei_end; ++ei) {
                    char label = automata[*ei].mark;
                    auto target = boost::target(*ei, automata);
                    targets[label].push_back(target);
                }

                for (const auto& [label, dests] : targets) {
                    if (dests.size() > 1) {
                        auto src = pair_to_vertex[make_ordered_pair(v1, v2)];
                        auto dst = pair_to_vertex[make_ordered_pair(dests[0], dests[1])];

                        if (!boost::edge(src, dst, g).second) {
                            boost::add_edge(src, dst, g);
                        }
                    }
                    else if (dests.size() == 1) {
                        auto src = pair_to_vertex[make_ordered_pair(v1, v2)];
                        auto dst = pair_to_vertex[make_ordered_pair(dests[0], dests[0])];

                        if (!boost::edge(src, dst, g).second) {
                            boost::add_edge(src, dst, g);
                        }
                    }
                }
            }
        }

        yield(g);
    });
}

template class BisetGraph<DirectedGraph>;
