#ifndef BISETGRAPH_H
#define BISETGRAPH_H

#include <Generators/IGenerator.h>
#include <types/types.h>

template <typename GraphType>
class BisetGraph : public IGenerator<DirectedGraph> {
private:
    GraphType& graph;
public:
    using pull_type = GeneratorTraits<DirectedGraph>::pull_type;

    BisetGraph(GraphType& g);
    pull_type generateGraphs() override;
};

#include "BisetGraph.tpp"

#endif
