#ifndef ALLTWOOUTGOINGEDGES_H
#define ALLTWOOUTGOINGEDGES_H

#include "Generators/IGenerator.h"


template <typename GraphType>
class AllTwoOutgoingEdges : public IGenerator<GraphType> {
private:
    size_t vertexes_count{};
    bool with_self_loops{true};
    static int parsePositiveInt(const std::string& input, const std::string& field_name);
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;

    AllTwoOutgoingEdges();
    pull_type generateGraphs() override;
};



#endif //TWOOUTGOINGEDGES_H
