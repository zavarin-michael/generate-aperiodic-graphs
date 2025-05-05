#ifndef TYPES_H
#define TYPES_H

#include <boost/graph/adjacency_list.hpp>
#include <boost/coroutine2/all.hpp>

struct VertexProperties {
    std::string node_id;
    std::string fillcolor;
};



using UndirectedGraph = boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, VertexProperties>;
using DirectedGraph = boost::adjacency_list<
    boost::listS,
    boost::vecS,
    boost::bidirectionalS,
    VertexProperties
>;
using GraphCoroutine = boost::coroutines2::coroutine<DirectedGraph&>;

struct BisetVertexProperties {
    std::string node_id;
    std::string fillcolor;
    int v1{};
    int v2{};
};

struct BisetGraphProperties {
    int meta;
};

using BisetGraph = boost::adjacency_list<
    boost::listS,
    boost::vecS,
    boost::bidirectionalS,
    BisetVertexProperties,
    BisetGraphProperties
>;
using BisetGraphCoroutine = boost::coroutines2::coroutine<BisetGraph&>;

// Marked edge
struct AutomataEdgeProperties {
    char mark;
};

using Automata = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::bidirectionalS,
    VertexProperties,
    AutomataEdgeProperties
>;
using AutomataCoroutine = boost::coroutines2::coroutine<Automata&>;

struct AutomataGenerationResult {
    long long mask;
    std::shared_ptr<Automata> automata;
};

using AutomataPairCoroutine = boost::coroutines2::coroutine<AutomataGenerationResult&>;

template<typename GraphType>
struct GeneratorTraits {
    using pull_type = boost::coroutines2::coroutine<GraphType&>::pull_type;
};

#endif