#ifndef IRECORDER_H
#define IRECORDER_H

#include "iostream"

template <typename GraphType>
class IRecorder {
public:
    virtual void recordGraph(GraphType& g, std::string name = "") = 0;
    virtual ~IRecorder() = default;
};

#endif
