#ifndef RANDOMTWOOUTGOINGEDGES_H
#define RANDOMTWOOUTGOINGEDGES_H

#include "Generators/IGenerator.h"


template <typename GraphType>
class RandomTwoOutgoingEdges : public IGenerator<GraphType> {
private:
    size_t vertexes_count{10};
    size_t graphs_count{10000};
    bool show_progress{true};
    bool with_self_loops{false};
    bool with_multiple_edges{false};
    static int parsePositiveInt(const std::string& input, const std::string& field_name);
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;
    RandomTwoOutgoingEdges();
    pull_type generateGraphs() override;
    size_t countGeneratedGraphs() override;
};



#endif //TWOOUTGOINGEDGES_H
