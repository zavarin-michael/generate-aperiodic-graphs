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
        std::cout << std::left << "-> Enter filename to read: ";
        std::getline(std::cin, path);
        dirPath = path;
    } else {
        dirPath = filepath;
    }

    if (!exists(dirPath)) {
        throw std::runtime_error("No such directory!");
    }
}


template<>
GraphCoroutine::pull_type MultipleGraphsReader<Graph>::read() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type& yield) {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                yield(*SingleGraphReader<Graph>(entry.path()).read().begin());
            }
        }
    });
}

template<>
AutomataCoroutine::pull_type MultipleGraphsReader<Automata>::read() {
    return AutomataCoroutine::pull_type([this](AutomataCoroutine::push_type& yield) {
        for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
            if (entry.is_regular_file()) {
                yield(*SingleGraphReader<Automata>(entry.path()).read().begin());
            }
        }
    });
}

template class MultipleGraphsReader<Graph>;
template class MultipleGraphsReader<Automata>;
