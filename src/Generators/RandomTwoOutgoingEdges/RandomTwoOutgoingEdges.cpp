#include "RandomTwoOutgoingEdges.h"

#include <iostream>
#include <random>
#include "types/types.h"

template<>
int RandomTwoOutgoingEdges<Graph>::parsePositiveInt(const std::string& input, const std::string& field_name) {
    std::istringstream iss(input);
    int value;
    if (!(iss >> value) || value <= 0) {
        throw std::invalid_argument("Invalid input for \"" + field_name + "\": must be a positive integer.");
    }
    return value;
}

template <>
RandomTwoOutgoingEdges<Graph>::RandomTwoOutgoingEdges() {
    std::string input;
    std::cout << "\n"
              << "+=====================================+\n"
              << "|        GENERATOR INITIALIZATION     |\n"
              << "+=====================================+\n\n";

    std::cout << std::left << "-> Enter number of vertexes[" << vertexes_count << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        vertexes_count = parsePositiveInt(input, "Vertexes count");
    }

    std::cout << std::left << "-> Enter count of graphs [" << graphs_count << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        graphs_count = parsePositiveInt(input, "Graphs count");
    }

    std::cout << std::left << "-> With self loops [" << (with_self_loops ? "true" : "false") << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        with_self_loops = !(input == "false");
    }
}

template <>
GraphCoroutine::pull_type RandomTwoOutgoingEdges<Graph>::generateGraphs() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type &yield) {
        std::random_device rd;
        std::mt19937 gen(rd());
        auto pair_vertices = std::vector<std::pair<int, int> >();

        for (auto i = 0; i < this->vertexes_count; ++i) {
            for (auto j = i; j < this->vertexes_count; ++j) {
                pair_vertices.emplace_back(i, j);
            }
        }
        std::uniform_int_distribution<> dist(0, pair_vertices.size() - 1);

        for (size_t i = 0; i < graphs_count; ++i) {
            auto graph = Graph(this->vertexes_count);

            for (int j = 0; j < vertexes_count; ++j) {
                bool run = !with_self_loops;
                do {
                    auto [v, u] = pair_vertices[dist(gen)];

                    if (v != j && u != j) {
                        add_edge(j, u, graph);
                        add_edge(j, v, graph);
                        run = false;
                    }
                } while (run);
            }

            auto [vi, vi_end] = boost::vertices(graph);
            for (auto it = vi; it != vi_end; ++it) {
                graph[*it].node_id = *it;
            }

            if (i % 100 == 0) {
                std::cout << i << "\n";
            }

            yield(graph);
        }
    });
}

template class RandomTwoOutgoingEdges<Graph>;
