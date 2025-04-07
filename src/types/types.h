#ifndef TYPES_H
#define TYPES_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/coroutine2/all.hpp>

struct VertexProperties {
    long long node_id;
    std::string fillcolor;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, VertexProperties>;
using GraphCoroutine = boost::coroutines2::coroutine<Graph&>;

// Marked edge
struct EdgeProperties {
    char mark;
};

using Automata = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    VertexProperties,
    EdgeProperties
>;
using AutomataCoroutine = boost::coroutines2::coroutine<std::pair<int, std::shared_ptr<Automata>>>;

#endif