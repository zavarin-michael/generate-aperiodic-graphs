#ifndef SINGLEGRAPHREADER_H
#define SINGLEGRAPHREADER_H

#include <types/types.h>
#include <fstream>
#include "Readers/IReader.h"

template <typename GraphType>
class SingleGraphReader: public IReader<GraphType> {
private:
    std::ifstream file;
public:
    SingleGraphReader(const std::filesystem::path& filepath = "");
    boost::coroutines2::coroutine<GraphType&>::pull_type read();
};


#endif //SINGLEGRAPHREADER_H
