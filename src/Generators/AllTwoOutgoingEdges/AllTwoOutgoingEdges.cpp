#include "AllTwoOutgoingEdges.h"

#include <iostream>
#include <boost/graph/copy.hpp>

#include "types/types.h"

template<>
int AllTwoOutgoingEdges<Graph>::parsePositiveInt(const std::string& input, const std::string& field_name) {
    std::istringstream iss(input);
    int value;
    if (!(iss >> value) || value <= 0) {
        throw std::invalid_argument("Invalid input for \"" + field_name + "\": must be a positive integer.");
    }
    return value;
}

template<>
AllTwoOutgoingEdges<Graph>::AllTwoOutgoingEdges() {
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
        with_self_loops = !(input == "false");
    }
}

template<>
GraphCoroutine::pull_type AllTwoOutgoingEdges<Graph>::generateGraphs() {
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

            auto initGraph = Graph(n);

            for (auto j : cases) {
                add_edge(j.first, j.second, initGraph);
            }

            bool done = false;

            while (true) {
                Graph graph;
                copy_graph(initGraph, graph);

                for (int i = 1; i < n; ++i) {
                    auto [v, u] = pair_vertices[indices[i]];

                    add_edge(i, u, graph);
                    add_edge(i, v, graph);
                }

                auto [vi, vi_end] = boost::vertices(graph);
                for (auto it = vi; it != vi_end; ++it) {
                    graph[*it].node_id = *it;
                }

                bool has_loops = std::any_of(
                    boost::edges(graph).first,
                    boost::edges(graph).second,
                    [&](auto e) {
                        return boost::source(e, graph) == boost::target(e, graph);
                    }
                );

                if (!has_loops || with_self_loops) {
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

template class AllTwoOutgoingEdges<Graph>;