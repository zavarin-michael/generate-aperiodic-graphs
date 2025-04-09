#include "Predicates.h"

#include "types/types.h"
#include <boost/graph/strong_components.hpp>
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
    Graph& graph,
    Graph::vertex_descriptor v,
    std::vector<bool>& visited,
    std::set<Graph::vertex_descriptor>& goal
) {
    if (goal.count(v)) {
        return true;
    }

    visited[v] = true;

    for (auto [ei, ei_end] = boost::out_edges(v, graph); ei != ei_end; ++ei) {
        auto u = boost::target(*ei, graph);
        if (!visited[u]) {
            if (dfs(graph, u, visited, goal)) {
                return true;
            }
        }
    }

    return false;
}

auto make_ordered_pair = [](auto a, auto b) {
    auto [min_v, max_v] = std::minmax(a, b);
    return std::make_pair(min_v, max_v);
};

std::pair<Graph, std::set<Graph::vertex_descriptor>> getBisetGraph(const Automata& automata) {
    Graph g;

    std::vector<Automata::vertex_descriptor> automata_vertices;
    for (auto [vi, vi_end] = boost::vertices(automata); vi != vi_end; ++vi) {
        automata_vertices.push_back(*vi);
    }

    // mapping to source edges
    std::map<std::pair<int, int>, Graph::vertex_descriptor> pair_to_vertex;
    std::set<Graph::vertex_descriptor> goal;

    // all edges from source graph
    for (auto v1 : automata_vertices) {
        for (auto v2 : automata_vertices) {
            auto p = make_ordered_pair(v1, v2);
            if (pair_to_vertex.contains(p))
                continue;
            VertexProperties props;
            props.node_id = v1 * 10 + v2;
            const auto vertex_in_g = boost::add_vertex(props, g);

            pair_to_vertex[p] = vertex_in_g;

            if (v1 == v2) {
                goal.insert(vertex_in_g);
            }
        }
    }

    for (auto v1 : automata_vertices) {
        for (auto v2 : automata_vertices) {
            std::map<char, std::vector<Automata::vertex_descriptor>> targets;

            // all transitions from v1
            for (auto [ei, ei_end] = boost::out_edges(v1, automata); ei != ei_end; ++ei) {
                char label = automata[*ei].mark;
                auto target = boost::target(*ei, automata);
                targets[label].push_back(target);
            }

            // all transitions from v2
            for (auto [ei, ei_end] = boost::out_edges(v2, automata); ei != ei_end; ++ei) {
                char label = automata[*ei].mark;
                auto target = boost::target(*ei, automata);
                targets[label].push_back(target);
            }

            for (const auto& [label, dests] : targets) {
                if (dests.size() > 1) {
                    boost::add_edge(
                        pair_to_vertex[make_ordered_pair(v1, v2)],
                        pair_to_vertex[make_ordered_pair(dests[0], dests[1])],
                        g
                    );
                }
                else if (dests.size() == 1) {
                    boost::add_edge(
                        pair_to_vertex[make_ordered_pair(v1, v2)],
                        pair_to_vertex[make_ordered_pair(dests[0], dests[0])],
                        g
                    );
                }
            }
        }
    }

    return {g, goal};
}

bool isSynchronized(const Automata& automata) {
    auto [graph, goal] = getBisetGraph(automata);

    for (auto [vi, vi_end] = boost::vertices(graph); vi != vi_end; ++vi) {
        std::vector<bool> visited(boost::num_vertices(graph), false);
        if (!dfs(graph, *vi, visited, goal)) {
            return false;
        }
    }
    return true;
}

template bool isStrongConnected(const Graph& graph);
template bool isStrongConnected(const Automata& automata);

template bool isAperiodic(const Graph& graph);
template bool isAperiodic(const Automata& automata);

template bool isNotAperiodic(const Graph& graph);
template bool isNotAperiodic(const Automata& automata);
