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
    DirectedGraph& graph;
    bool isHalf = false;
    std::vector<int> masks = {};
    void generateAutomata(long long mask, Automata& automata);
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;
    AutomatasFromGraph(DirectedGraph& g, bool userInput, bool isHalf = false, std::vector<int> masks = {});
    pull_type generateGraphs() override;
};


#endif //AUTOMATASFROMGRAPH_H
