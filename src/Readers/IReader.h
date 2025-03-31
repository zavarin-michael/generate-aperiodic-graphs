#ifndef IREADER_H
#define IREADER_H

template <typename GraphType>
class IReader {
public:
    virtual GraphType readGraph() = 0;
    virtual ~IReader() = default;
};

#endif //IREADER_H
