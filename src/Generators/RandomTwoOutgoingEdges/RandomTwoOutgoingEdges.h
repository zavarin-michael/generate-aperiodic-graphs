#ifndef RANDOMTWOOUTGOINGEDGES_H
#define RANDOMTWOOUTGOINGEDGES_H

#include "Generators/IGenerator.h"


class RandomTwoOutgoingEdges : public IGenerator {
private:
    size_t vertexes_count{10};
    size_t graphs_count{10000};
    bool with_self_loops{true};
    int parsePositiveInt(const std::string& input, const std::string& field_name);
public:
    RandomTwoOutgoingEdges();
    GraphCoroutine::pull_type generateGraphs() override;
};



#endif //TWOOUTGOINGEDGES_H
