#include "RandomTwoOutgoingEdges.h"

#include <iostream>
#include <random>
#include "types/types.h"
#include "Utils/Utils.h"

template <>
RandomTwoOutgoingEdges<DirectedGraph>::RandomTwoOutgoingEdges() {
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

    std::cout << std::left << "-> With multiple edges [" << (with_multiple_edges ? "true" : "false") << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        with_multiple_edges = input == "true";
    }

    std::cout << std::left << "-> Show progress [" << (show_progress ? "true" : "false") << "]: ";
    std::getline(std::cin, input);

    if (!input.empty()) {
        show_progress = input != "false";
    }
}

template <>
GraphCoroutine::pull_type RandomTwoOutgoingEdges<DirectedGraph>::generateGraphs() {
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

        auto start = std::chrono::high_resolution_clock::now();

        size_t i = 1;
        while (true) {
            auto graph = DirectedGraph(this->vertexes_count);
            bool run;

            int subtrahend = 0;
            if (with_self_loops) {
                auto [v, u] = pair_vertices[dist(gen)];
                add_edge(subtrahend, subtrahend, graph);
                add_edge(subtrahend, v, graph);
                subtrahend += 1;
            }
            if (with_multiple_edges) {
                auto [v, u] = pair_vertices[dist(gen)];
                add_edge(subtrahend, v, graph);
                add_edge(subtrahend, v, graph);
                subtrahend += 1;
            }

            for (int j = subtrahend; j < vertexes_count; ++j) {
                do {
                    auto [v, u] = pair_vertices[dist(gen)];
                    run = false;

                    run |= !with_self_loops && (v == j || u == j);
                    run |= !with_multiple_edges && v == u;

                    if (!run) {
                        add_edge(j, u, graph);
                        add_edge(j, v, graph);
                    }
                } while (run);
            }

            auto [vi, vi_end] = boost::vertices(graph);
            for (auto it = vi; it != vi_end; ++it) {
                graph[*it].node_id = getVertexName(*it);
            }

            if (i % 5000 == 0) {
                printProgress("Graphs progress:", i, graphs_count, start);
            }
            yield(graph);
            i++;

            if (i >= graphs_count + 1) {
                break;
            }
        }
    });
}

template<>
size_t RandomTwoOutgoingEdges<DirectedGraph>::countGeneratedGraphs() {
    return graphs_count;
}

template class RandomTwoOutgoingEdges<DirectedGraph>;
