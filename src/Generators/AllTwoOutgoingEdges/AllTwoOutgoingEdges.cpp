#include "AllTwoOutgoingEdges.h"

#include <iostream>
#include <boost/graph/copy.hpp>

#include "types/types.h"
#include "Utils/Utils.h"

template<>
AllTwoOutgoingEdges<DirectedGraph>::AllTwoOutgoingEdges() {
    std::string input;
    std::cout << "\n"
              << "+=====================================+\n"
              << "|        GENERATOR INITIALIZATION     |\n"
              << "+=====================================+\n\n";

    std::cout << std::left << "-> Enter number of vertexes: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        vertexes_count = parsePositiveInt(input, "Vertexes count");
    }

    std::cout << std::left << "-> With self loops [" << (with_self_loops ? "true" : "false") << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        with_self_loops = input != "false";
    }

    std::cout << std::left << "-> Show progress[" << (show_progress ? "true" : "false") << "]: ";
    std::getline(std::cin, input);

    if (!input.empty()) {
        show_progress = input != "false";
    }
}

template<>
GraphCoroutine::pull_type AllTwoOutgoingEdges<DirectedGraph>::generateGraphs() {
    auto n = vertexes_count;
    return GraphCoroutine::pull_type([n, this](GraphCoroutine::push_type& yield) {
        auto pair_vertices = std::vector<std::pair<int, int>>();

        for (auto i = 0; i < n; ++i) {
            for (auto j = i; j < n; ++j) {
                pair_vertices.emplace_back(i, j);
            }
        }

        std::vector<std::vector<std::pair<int, int>>> edges {
            {{0, 1}, {0, 0}},
            {{0, 1}, {0, 2}},
            {{0, 1}, {0, 1}},
        };

        for (auto cases : edges) {
            std::vector<size_t> indices(n, 0);

            auto initGraph = DirectedGraph(n);

            for (auto j : cases) {
                add_edge(j.first, j.second, initGraph);
            }

            bool done = false;

            while (true) {
                DirectedGraph graph;
                copy_graph(initGraph, graph);

                for (int i = 1; i < n; ++i) {
                    auto [v, u] = pair_vertices[indices[i]];

                    add_edge(i, u, graph);
                    add_edge(i, v, graph);
                }

                auto [vi, vi_end] = boost::vertices(graph);
                for (auto it = vi; it != vi_end; ++it) {
                    graph[*it].node_id = getVertexName(*it);
                }

                bool has_loops = std::any_of(
                    boost::edges(graph).first,
                    boost::edges(graph).second,
                    [&](auto e) {
                        return boost::source(e, graph) == boost::target(e, graph);
                    }
                );

                if (!has_loops || with_self_loops) {
                    graphs_count++;
                    if (show_progress && graphs_count % 10000 == 0) {
                        std::cout << graphs_count << "\n";
                    }
                    yield(graph);
                }

                for (int i = n - 1; i > 0; --i) {
                    if (indices[i] < pair_vertices.size() - 1) {
                        indices[i]++;
                        for (int j = i + 1; j < n; ++j) {
                            indices[j] = 0;
                        }
                        break;
                    }
                    if (i == 1) {
                        done = true;
                    }
                }

                if (done) {
                    break;
                }
            }
        }
    });
}

template<>
size_t AllTwoOutgoingEdges<DirectedGraph>::countGeneratedGraphs() {
    return graphs_count;
}

template class AllTwoOutgoingEdges<DirectedGraph>;