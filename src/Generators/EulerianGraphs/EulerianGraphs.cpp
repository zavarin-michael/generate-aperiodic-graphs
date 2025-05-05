#include "EulerianGraphs.h"
#include <iostream>
#include <random>
#include <algorithm>
#include <set>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>

#include "types/types.h"
#include "Utils/Utils.h"

template<typename S>
auto select_random(const S &s, size_t n) {
    auto it = std::begin(s);
    // 'advance' the iterator n times
    std::advance(it,n);
    return it;
}

template<>
int EulerianGraphs<DirectedGraph>::parsePositiveInt(const std::string& input, const std::string& field_name) {
    std::istringstream iss(input);
    int value;
    if (!(iss >> value) || value <= 0) {
        throw std::invalid_argument("Invalid input for \"" + field_name + "\": must be a positive integer.");
    }
    return value;
}


boost::graph_traits<DirectedGraph>::vertex_descriptor find_vertex_with_in_degree_one(DirectedGraph& graph) {
    auto [v_begin, v_end] = vertices(graph);

    for (auto it = v_begin; it != v_end; ++it) {
        if (in_degree(*it, graph) == 1) {
            return *it;
        }
    }

    auto rec = new ConsoleRecorder<DirectedGraph>();
    rec->recordGraph(graph, "");

    throw std::runtime_error("No vertex with in_degree == 1 found.");
}

boost::graph_traits<DirectedGraph>::vertex_descriptor find_vertex_with_out_degree_one(DirectedGraph& graph) {
    auto [v_begin, v_end] = vertices(graph);

    for (auto it = v_begin; it != v_end; ++it) {
        if (out_degree(*it, graph) == 1) {
            return *it;
        }
    }

    throw std::runtime_error("No vertex with out_degree == 1 found.");
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

    std::cout << std::left << "-> With multiple edges [" << (with_multiple_edges ? "true" : "false") << "]: ";
    std::getline(std::cin, input);
    if (!input.empty()) {
        with_multiple_edges = input == "true";
    }
}

template <>
GraphCoroutine::pull_type EulerianGraphs<DirectedGraph>::generateGraphs() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type &yield) {
        std::random_device rd;
        std::mt19937 gen(rd());

        for (size_t i = 0; i < graphs_count; ++i) {
            bool br = false;
            auto graph = DirectedGraph(this->vertexes_count);
            auto possible_v = std::set<long long>();
            long long current_v = *vertices(graph).first;
            auto [vi, vi_end] = boost::vertices(graph);
            auto count = 0;
            for (auto it = vi; it != vi_end; ++it) {
                possible_v.insert(*it);
                graph[*it].node_id = getVertexName(count++);
            }

            for (int j = 0; j < vertexes_count * 2 - 1; ++j) {
                auto erased = std::set<long long>();
                if (!with_self_loops || out_degree(current_v, graph) == 1) {
                    auto k = possible_v.erase(current_v);
                    if (k > 0) {
                        erased.insert(current_v);
                    }
                }

                if (!with_multiple_edges && out_degree(current_v, graph) == 1) {
                    auto [out_i, out_end] = out_edges(current_v, graph);
                    auto k = possible_v.erase(target(*out_i, graph));
                    if (k > 0) {
                        erased.insert(target(*out_i, graph));
                    }
                }

                if (possible_v.size() == 0) {
                    br = true;
                    break;
                }

                std::uniform_int_distribution<> dist(0, static_cast<int>(possible_v.size()) - 1);

                long long new_v = *select_random(possible_v, dist(rd));

                for (auto i : erased) {
                    possible_v.insert(i);
                }

                add_edge(current_v, new_v, graph);
                if (out_degree(current_v, graph) > 1) {
                    possible_v.erase(current_v);
                }
                if (in_degree(new_v, graph) > 1) {
                    possible_v.erase(new_v);
                }
                current_v = new_v;
            }

            if (!br) {
                bool take = true;
                auto from = find_vertex_with_out_degree_one(graph);
                auto to = find_vertex_with_in_degree_one(graph);
                auto [out_i, out_end] = out_edges(from, graph);
                take &= with_multiple_edges || target(*out_i, graph) != to;
                take &= !with_self_loops || to != from;
                if (take) {
                    add_edge(from, to, graph);
                    yield(graph);
                }
            }
        }
    });
}

template class EulerianGraphs<DirectedGraph>;
