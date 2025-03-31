#include "AllTwoOutgoingEdges.h"

#include <iostream>

#include "types/types.h"

AllTwoOutgoingEdges::AllTwoOutgoingEdges() {
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
}

GraphCoroutine::pull_type AllTwoOutgoingEdges::generateGraphs() {
    auto n = vertexes_count;
    return GraphCoroutine::pull_type([n](GraphCoroutine::push_type& yield) {
        auto pair_vertices = std::vector<std::pair<int, int>>();

        for (auto i = 0; i < n; ++i) {
            for (auto j = i; j < n; ++j) {
                pair_vertices.emplace_back(i, j);
            }
        }

        std::vector<size_t> indices(n, 0);

        bool done = false;

        while (true) {
            auto graph = Graph(n);

            for (int i = 0; i < n; ++i) {
                auto [v, u] = pair_vertices[indices[i]];

                add_edge(i, u, graph);
                add_edge(i, v, graph);
            }

            auto [vi, vi_end] = boost::vertices(graph);
            for (auto it = vi; it != vi_end; ++it) {
                graph[*it].node_id = *it;
            }

            yield(graph);

            for (int i = n - 1; i >= 0; --i) {
                if (indices[i] < pair_vertices.size() - 1) {
                    indices[i]++;
                    for (int j = i + 1; j < n; ++j) {
                        indices[j] = 0;
                    }
                    break;
                }
                if (i == 0) {
                    done = true;
                }
            }

            if (done) {
                break;
            }
        }
    });
}

int AllTwoOutgoingEdges::parsePositiveInt(const std::string& input, const std::string& field_name) {
    std::istringstream iss(input);
    int value;
    if (!(iss >> value) || value <= 0) {
        throw std::invalid_argument("Invalid input for \"" + field_name + "\": must be a positive integer.");
    }
    return value;
}