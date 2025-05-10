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


    std::cout << std::left << "-> Show progress [" << (show_progress ? "true" : "false") << "]: ";
    std::getline(std::cin, input);

    if (!input.empty()) {
        show_progress = input != "false";
    }
}

template <>
GraphCoroutine::pull_type EulerianGraphs<DirectedGraph>::generateGraphs() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type &yield) {
        std::random_device rd;
        std::mt19937 gen(rd());
        size_t i = 1;

        auto start = std::chrono::high_resolution_clock::now();

        while (true) {
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

            int subtrahend = 0;
            bool is_added_multiple = false;
            if (with_self_loops) {
                add_edge(0, 0, graph);
                subtrahend += 1;
            }


            for (int j = subtrahend; j < vertexes_count * 2 - 1; ++j) {
                auto erased = std::set<long long>();
                if (!with_self_loops || out_degree(current_v, graph) == 1) {
                    auto k = possible_v.erase(current_v);
                    if (k > 0) {
                        erased.insert(current_v);
                    }
                }

                if (out_degree(current_v, graph) == 1) {
                    auto [out_i, out_end] = out_edges(current_v, graph);
                    auto dest = target(*out_i, graph);
                    if (!with_multiple_edges) {
                        auto k = possible_v.erase(dest);
                        if (k > 0) {
                            erased.insert(dest);
                        }
                    } else {
                        if (!is_added_multiple && possible_v.contains(dest)) {
                            erased.insert(possible_v.begin(), possible_v.end());
                            possible_v.clear();
                            possible_v.insert(dest);
                            is_added_multiple = true;
                        }
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
                    if (i % 5000 == 0) {
                        printProgress("Graphs progress:", i, graphs_count, start);
                    }
                    yield(graph);
                    i++;
                    if (i >= graphs_count + 1) {
                        break;
                    }
                }
            }
        }
    });
}

template<>
size_t EulerianGraphs<DirectedGraph>::countGeneratedGraphs() {
    return graphs_count;
}

template class EulerianGraphs<DirectedGraph>;
