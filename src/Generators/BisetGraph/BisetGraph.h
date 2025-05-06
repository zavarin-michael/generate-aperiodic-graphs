#ifndef BISETGRAPH_H
#define BISETGRAPH_H

#include <Generators/IGenerator.h>
#include <types/types.h>

template <typename GraphType>
class BisetGraphGenerator : public IGenerator<BisetGraph> {
private:
    GraphType& graph;
public:
    using pull_type = GeneratorTraits<BisetGraph>::pull_type;

    BisetGraphGenerator(GraphType& g);
    pull_type generateGraphs() override;
    size_t countGeneratedGraphs() override;
};

#include "BisetGraph.tpp"

#endif
