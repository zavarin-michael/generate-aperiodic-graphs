#ifndef IREADER_H
#define IREADER_H

#include "types/types.h"

template <typename GraphType>
class IReader {
public:
    virtual typename boost::coroutines2::coroutine<GraphType&>::pull_type read() = 0;
     virtual ~IReader() = default;
};

#endif //IREADER_H
