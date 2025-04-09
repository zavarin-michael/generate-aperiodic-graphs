//
// Created by Михаил Заварин on 09.04.2025.
//

#ifndef AUTOMATASFROMGRAPH_H
#define AUTOMATASFROMGRAPH_H
#include <Generators/IGenerator.h>
#include <types/types.h>

template <typename GraphType>
class AutomatasFromGraph : public IGenerator<GraphType> {
private:
    Graph& graph;
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;

    AutomatasFromGraph(Graph& g);
    pull_type generateGraphs() override;
};


#endif //AUTOMATASFROMGRAPH_H
