#ifndef TYPES_H
#define TYPES_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/coroutine2/all.hpp>

struct VertexProperties {
    long long node_id;
    std::string fillcolor;
};

using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties>;
using DirectedGraph = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS, VertexProperties>;
using GraphCoroutine = boost::coroutines2::coroutine<DirectedGraph&>;

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
using AutomataCoroutine = boost::coroutines2::coroutine<Automata&>;

struct AutomataGenerationResult {
    int mask;
    std::shared_ptr<Automata> automata;
};

using AutomataPairCoroutine = boost::coroutines2::coroutine<AutomataGenerationResult&>;

template<typename GraphType>
struct GeneratorTraits {
    using pull_type = boost::coroutines2::coroutine<GraphType&>::pull_type;
};

#endif