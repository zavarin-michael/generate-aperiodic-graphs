#ifndef IGENERATOR_H
#define IGENERATOR_H

#include "types/types.h"

class IGenerator {
public:
    virtual GraphCoroutine::pull_type generateGraphs() = 0;
    virtual ~IGenerator() = default;
};

#endif
