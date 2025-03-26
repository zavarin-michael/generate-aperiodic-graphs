#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/strong_components.hpp>
#include <numeric>
#include <filesystem>
#include <fstream>

#include "types/types.h"
#include "Recorders/DiskRecorder/DiskRecorder.h"

const std::string GRAPH_DIR = "graphs";

int countStrongComponents(const Graph& graph) {
    std::vector<int> component(num_vertices(graph));
    return strong_components(graph, make_iterator_property_map(component.begin(), get(boost::vertex_index, graph)));
}

bool isAperiodic(const Graph& graph, int v, int depth, std::vector<int> &visited, int &acc) {
    if (visited[v] != -1) {
        depth--;
        const auto diff = visited[v] - depth - 1;
        acc = std::gcd(diff, acc);
    } else {
        visited[v] = depth;
        auto [fst, last] = adjacent_vertices(v, graph);

        for (auto it = fst; it != last; ++it) {
            if (isAperiodic(graph, *it, depth + 1, visited, acc)) {
                return true;
            }
        }
    }

    return acc == 1;
}

void print_graph(const Graph& g, int n) {
    std::cout << "Graph:" << '\n';
    for (int v = 0; v < n; ++v) {
        std::cout << v << ": ";
        // Получаем диапазон исходящих рёбер для вершины v.
        auto edges = out_edges(v, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            std::cout << target_vertex << " ";
        }
        std::cout << '\n';
    }
    std::cout << "------------------------" << '\n';
}

int main() {
    int n = 3;
    std::cout << "Enter number of vertexes" << '\n';
    std::cin >> n;

    auto nString = std::to_string(n);
    auto recorder = DiskRecorder();

    auto pair_vertices = std::vector<std::pair<int, int>>();

    for (auto i = 0; i < n; ++i) {
        for (auto j = i; j < n; ++j) {
            pair_vertices.push_back(std::make_pair(i, j));
        }
    }

    std::vector<size_t> indices(n, 0);

    bool done = false;
    auto count = 0;
    while (true) {
        auto graph = Graph(n);

        bool skip = false;
        for (int i = 0; i < n; ++i) {
            auto [v, u] = pair_vertices[indices[i]];
            // if (v == i || u == i) {
            //     skip = true;
            //     break;
            // }
            add_edge(i, u, graph);
            add_edge(i, v, graph);
        }

        if (!skip) {
            auto vis = new std::vector(n, -1);
            int acc = 2 * 3 * 5 * 7 * 11;
            if (countStrongComponents(graph) == 1) {
                if (!isAperiodic(graph, 0, 0, *vis, acc)) {
                    recorder.recordGraph(graph);
                    count++;
                }

                // cout << acc << endl;
                // print_graph(graph, n);
            }
        }

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

    std::cout << count << '\n';
    return 0;
}
