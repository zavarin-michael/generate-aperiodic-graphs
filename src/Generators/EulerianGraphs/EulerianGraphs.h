//
// Created by User on 18.04.2025.
//

#ifndef EULERIANGRAPHS_H
#define EULERIANGRAPHS_H


#include "Generators/IGenerator.h"


template <typename GraphType>
class EulerianGraphs : public IGenerator<GraphType> {
private:
    size_t vertexes_count{10};
    size_t graphs_count{10000};
    bool show_progress{true};
    bool with_self_loops{false};
    bool with_multiple_edges{false};
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;
    EulerianGraphs();
    pull_type generateGraphs() override;
    size_t countGeneratedGraphs() override;
};


#endif //EULERIANGRAPHS_H
