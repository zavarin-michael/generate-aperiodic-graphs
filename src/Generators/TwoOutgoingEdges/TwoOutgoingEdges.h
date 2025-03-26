#ifndef TWOOUTGOINGEDGES_H
#define TWOOUTGOINGEDGES_H

#include "Generators/IGenerator.h"


class TwoOutgoingEdges : public IGenerator {
private:
    size_t vertexes_count;
public:
    TwoOutgoingEdges(int vertexes_count);
    GraphCoroutine::pull_type generateGraphs() override;
};



#endif //TWOOUTGOINGEDGES_H
