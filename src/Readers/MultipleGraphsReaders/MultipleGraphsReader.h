//
// Created by Михаил Заварин on 09.04.2025.
//

#ifndef MULTIPLEGRAPHSREADER_H
#define MULTIPLEGRAPHSREADER_H

#include <filesystem>
#include <Readers/IReader.h>


template <typename GraphType>
class MultipleGraphsReader: public IReader<GraphType> {
private:
    std::filesystem::path dirPath;
public:
    MultipleGraphsReader(std::filesystem::path filepath = "");
    boost::coroutines2::coroutine<GraphType&>::pull_type read();
};



#endif //MULTIPLEGRAPHSREADER_H
