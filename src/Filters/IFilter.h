#ifndef IFILTER_H
#define IFILTER_H
#include <types/types.h>

template <typename GraphType>
class IFilter {
public:
    virtual bool isAccepted(GraphType& graph) = 0;
    virtual ~IFilter() = default;
};

#endif
