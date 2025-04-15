#include "Predicates.h"

#include "types/types.h"
#include <boost/graph/strong_components.hpp>
#include "Generators/BisetGraph/BisetGraph.h"
#include <numeric>

template <typename GraphType>
int gcdCycles(const GraphType& graph, int v, int depth, std::vector<int> &visited, int acc) {
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

template <typename GraphType>
bool isAperiodic(const GraphType& graph) {
    auto vis = new std::vector(graph.m_vertices.capacity(), -1);
    int acc = 2 * 3 * 5 * 7 * 11;
    return gcdCycles(graph, 0, 0, *vis, acc) == 1;
}

template <typename GraphType>
bool isNotAperiodic(const GraphType& graph) {
    return not isAperiodic(graph);
}

template <typename GraphType>
bool isStrongConnected(const GraphType& graph) {
    std::vector<int> component(num_vertices(graph));
    return strong_components(graph, make_iterator_property_map(component.begin(), get(boost::vertex_index, graph))) == 1;
}

bool dfs(
    DirectedGraph& graph,
    DirectedGraph::vertex_descriptor v,
    std::vector<bool>& visited
) {
    if (graph[v].fillcolor == "green") {
        return true;
    }

    visited[v] = true;

    for (auto [ei, ei_end] = boost::out_edges(v, graph); ei != ei_end; ++ei) {
        auto u = boost::target(*ei, graph);
        if (!visited[u]) {
            if (dfs(graph, u, visited)) {
                return true;
            }
        }
    }

    return false;
}

bool isSynchronized(const Automata& automata) {
    Automata temp = automata;
    DirectedGraph graph = *BisetGraph<DirectedGraph>(temp).generateGraphs().begin();

    for (auto [vi, vi_end] = boost::vertices(graph); vi != vi_end; ++vi) {
        std::vector<bool> visited(boost::num_vertices(graph), false);
        if (!dfs(graph, *vi, visited)) {
            return false;
        }
    }
    return true;
}

template bool isStrongConnected(const DirectedGraph& graph);
template bool isStrongConnected(const Automata& automata);

template bool isAperiodic(const DirectedGraph& graph);
template bool isAperiodic(const Automata& automata);

template bool isNotAperiodic(const DirectedGraph& graph);
template bool isNotAperiodic(const Automata& automata);
