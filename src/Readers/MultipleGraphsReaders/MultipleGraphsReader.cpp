//
// Created by Михаил Заварин on 09.04.2025.
//

#include "MultipleGraphsReader.h"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include <filesystem>
#include <Readers/SingleGraphReader/SingleGraphReader.h>

template<typename GraphType>
MultipleGraphsReader<GraphType>::MultipleGraphsReader(std::filesystem::path filepath) {
    if (filepath.empty()) {
        std::string path;
        std::cout << "\n"
                  << "+=====================================+\n"
                  << "|       INITIALIZATION OF READER      |\n"
                  << "+=====================================+\n\n";
        std::cout << "!!!Warning!!! \n"
                  << "Your files in directory must have extension .dot to be read\n";
        std::cout << std::left << "-> Enter filename to read:";
        std::getline(std::cin, path);
        dirPath = path;
    } else {
        dirPath = filepath;
    }

    if (!exists(dirPath)) {
        throw std::runtime_error("No such directory!");
    }
}


template<typename GraphType>
boost::coroutines2::coroutine<GraphType &>::pull_type MultipleGraphsReader<GraphType>::read() {
    return typename boost::coroutines2::coroutine<GraphType &>::pull_type([this](boost::coroutines2::coroutine<GraphType &>::push_type& yield) {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".dot") {
                yield(*SingleGraphReader<GraphType>(entry.path()).read().begin());
            }
        }
    });
}

template class MultipleGraphsReader<DirectedGraph>;
template class MultipleGraphsReader<Automata>;
