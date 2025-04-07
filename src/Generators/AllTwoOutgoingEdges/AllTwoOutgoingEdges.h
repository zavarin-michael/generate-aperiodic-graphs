#ifndef ALLTWOOUTGOINGEDGES_H
#define ALLTWOOUTGOINGEDGES_H

#include "Generators/IGenerator.h"


class AllTwoOutgoingEdges : public IGenerator {
private:
    size_t vertexes_count{};
    bool with_self_loops{true};
    int parsePositiveInt(const std::string& input, const std::string& field_name);
public:
    AllTwoOutgoingEdges();
    GraphCoroutine::pull_type generateGraphs() override;
};



#endif //TWOOUTGOINGEDGES_H
