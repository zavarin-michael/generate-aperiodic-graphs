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
    SingleGraphReader(std::filesystem::path filepath = "");
    GraphType readGraph();
};


#endif //SINGLEGRAPHREADER_H
