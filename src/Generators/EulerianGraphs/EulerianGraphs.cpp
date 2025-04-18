#include "EulerianGraphs.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <set>
#include "types/types.h"

template<typename S>
auto select_random(const S &s, size_t n) {
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it,n);
    return it;
}

template <>
EulerianGraphs<DirectedGraph>::EulerianGraphs() {
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
GraphCoroutine::pull_type EulerianGraphs<DirectedGraph>::generateGraphs() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type &yield) {
        std::random_device rd;
        std::mt19937 gen(rd());

        for (size_t i = 0; i < graphs_count; ++i) {
            auto graph = DirectedGraph(this->vertexes_count);
            auto possible_v = std::set<long long int>();
            auto current_v = *vertices(graph).first;
            possible_v.insert(current_v);

            for (int j = 0; j < vertexes_count * 2; ++j) {
                if (possible_v.empty()) {
                    std::cout << j << " No valid vertices left.\n";
                    break;
                }
                std::uniform_int_distribution<> dist(0, static_cast<int>(possible_v.size()) - 1);

                auto new_v = *select_random(possible_v, dist(rd));
                add_edge(current_v, new_v, graph);
                if (out_degree(current_v, graph) > 1) {
                    possible_v.erase(current_v);
                }
                if (in_degree(new_v, graph) > 1) {
                    possible_v.erase(new_v);
                }
                current_v = new_v;
            }


            if (i % 10000 == 0) {
                std::cout << i << "\n";
            }

            yield(graph);
        }
    });
}

template class EulerianGraphs<DirectedGraph>;
