#ifndef IRECORDER_H
#define IRECORDER_H


#include "types/types.h"

class IRecorder {
public:
    virtual void recordGraph(const Graph& g) = 0;
    virtual ~IRecorder() = default;
};

#endif
