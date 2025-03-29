#include "Predicates.h"

#include "types/types.h"
#include <boost/graph/strong_components.hpp>
#include <numeric>

int gcdCycles(const Graph& graph, int v, int depth, std::vector<int> &visited, int acc) {
    if (visited[v] != -1) {
        depth--;
        const auto diff = visited[v] - depth - 1;
        acc = std::gcd(diff, acc);
    } else {
        visited[v] = depth;
        auto [fst, last] = adjacent_vertices(v, graph);

        for (auto it = fst; it != last; ++it) {
            acc = gcdCycles(graph, *it, depth + 1, visited, acc);
        }
    }

    return acc;
}

bool isAperiodic(const Graph& graph) {
    auto vis = new std::vector(graph.m_vertices.capacity(), -1);
    int acc = 2 * 3 * 5 * 7 * 11;
    return gcdCycles(graph, 0, 0, *vis, acc) == 1;
}

bool isNotAperiodic(const Graph& graph) {
    return not isAperiodic(graph);
}

bool isStrongConnected(const Graph& graph) {
    std::vector<int> component(num_vertices(graph));
    return strong_components(graph, make_iterator_property_map(component.begin(), get(boost::vertex_index, graph))) == 1;
}