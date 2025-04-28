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
    bool with_self_loops{true};
    bool with_multiple_edges{false};
    static int parsePositiveInt(const std::string& input, const std::string& field_name);
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;
    EulerianGraphs();
    pull_type generateGraphs() override;
};


#endif //EULERIANGRAPHS_H
