#ifndef IFILTER_H
#define IFILTER_H
#include <types/types.h>

class IFilter {
public:
    virtual bool isAccepted(Graph graph) = 0;
    virtual ~IFilter() = default;
};

#endif
