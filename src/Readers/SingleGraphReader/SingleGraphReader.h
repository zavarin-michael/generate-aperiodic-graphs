#ifndef SINGLEGRAPHREADER_H
#define SINGLEGRAPHREADER_H

#include <types/types.h>


class SingleGraphReader {
public:
    Graph readGraph(const std::filesystem::path& filepath);
};


#endif //SINGLEGRAPHREADER_H
