#ifndef BISETGRAPH_H
#define BISETGRAPH_H

#include <Generators/IGenerator.h>
#include <types/types.h>

template <typename GraphType>
class BisetGraph : public IGenerator<GraphType> {
private:
    Automata& automata;
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;

    BisetGraph(Automata& g);
    pull_type generateGraphs() override;
};


#endif
