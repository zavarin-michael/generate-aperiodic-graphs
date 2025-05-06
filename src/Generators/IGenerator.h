#ifndef IGENERATOR_H
#define IGENERATOR_H

#include "types/types.h"

template <typename GraphType>
class IGenerator {
public:
    using pull_type = typename GeneratorTraits<GraphType>::pull_type;

    virtual pull_type generateGraphs() = 0;
    virtual size_t countGeneratedGraphs() = 0;
    virtual ~IGenerator() = default;
};

#endif
