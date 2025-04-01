#ifndef IRECORDER_H
#define IRECORDER_H

template <typename GraphType>
class IRecorder {
public:
    virtual void recordGraph(GraphType& g) = 0;
    virtual ~IRecorder() = default;
};

#endif
