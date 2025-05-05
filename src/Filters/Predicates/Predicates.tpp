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

template <typename GraphType>
bool isEulerian(GraphType& g) {
    if (!isStrongConnected(g))
        return false;

    auto [vi, vi_end] = vertices(g);
    for (auto it = vi; it != vi_end; ++it) {
        if (in_degree(*it, g) != out_degree(*it, g)) {
            return false;
        }
    }

    return true;
}

long long dfs(
    BisetGraph& graph,
    BisetGraph::vertex_descriptor v,
    std::vector<bool>& visited
) {
    if (visited[v]) {
        return 0;
    }

    visited[v] = true;
    long long count = 1;

    for (auto [ei, ei_end] = boost::in_edges(v, graph); ei != ei_end; ++ei) {
        auto u = boost::source(*ei, graph);
        count += dfs(graph, u, visited);
    }

    return count;
}

bool isSynchronized(const Automata& automata) {
    Automata temp = automata;
    BisetGraph graph = *BisetGraphGenerator(temp).generateGraphs().begin();

    std::vector<bool> visited(boost::num_vertices(graph), false);
    auto vp = boost::vertices(graph);
    BisetGraph::vertex_descriptor v;
    for (auto vit = vp.first; vit != vp.second; ++vit) {
        if (graph[*vit].fillcolor == "green") {
            v = *vit;
            break;
        }
    }
    auto count = dfs(graph, v, visited);
    return count == graph.m_vertices.size();
}

long long getBits(long long mask, int v1, int v2) {
    auto bit1 = (mask >> v1) & 1;
    auto bit2 = (mask >> v2) & 1;
    auto result = (bit1 << 1) | bit2;
    return result;
}

long long dfs2(
    BisetGraph& graph,
    BisetGraph::vertex_descriptor v,
    std::vector<bool>& visited,
    long long mask
) {
    if (visited[v]) {
        return 0;
    }
    visited[v] = true;

    long long count = 1;
    for (auto [ei, ei_end] = boost::in_edges(v, graph); ei != ei_end; ++ei) {
        auto u = boost::source(*ei, graph);
        auto gMask = getBits(mask, graph[u].v1, graph[u].v2);
        if (graph[*ei].meta == gMask) {
            count += dfs2(graph, u, visited, mask);
        }
    }

    return count;
}

bool isSynchronizedNew(BisetGraph& graph, long long mask) {
    std::vector<bool> visited(boost::num_vertices(graph), false);

    auto vp = boost::vertices(graph);
    BisetGraph::vertex_descriptor v;
    for (auto vit = vp.first; vit != vp.second; ++vit) {
        if (graph[*vit].fillcolor == "green") {
            v = *vit;
            break;
        }
    }
    auto count = dfs2(graph, v, visited, mask);
    return count == graph.m_vertices.size();
}

template bool isStrongConnected(const DirectedGraph& graph);
template bool isStrongConnected(const Automata& automata);

template bool isAperiodic(const DirectedGraph& graph);
template bool isAperiodic(const Automata& automata);

template bool isNotAperiodic(const DirectedGraph& graph);
template bool isNotAperiodic(const Automata& automata);

template bool isEulerian(const Automata& automata);
template bool isEulerian(const DirectedGraph& automata);
