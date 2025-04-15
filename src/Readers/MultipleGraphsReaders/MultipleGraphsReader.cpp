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


template<typename GraphType>
boost::coroutines2::coroutine<GraphType &>::pull_type MultipleGraphsReader<GraphType>::read() {
    std::vector<std::filesystem::directory_entry> entries;

    // Step 1: collect all files
    for (const auto& entry : std::filesystem::directory_iterator(dirPath)) {
        if (entry.is_regular_file()) {
            entries.push_back(entry);
        }
    }

    std::sort(entries.begin(), entries.end(), [](const std::filesystem::directory_entry& a, const std::filesystem::directory_entry& b) {
        int numA = std::stoi(a.path().stem().string());
        int numB = std::stoi(b.path().stem().string());
        return numA < numB;
    });

    return typename boost::coroutines2::coroutine<GraphType &>::pull_type([entries](boost::coroutines2::coroutine<GraphType &>::push_type& yield) {
        for (const auto& entry : entries) {
            if (entry.is_regular_file()) {
                yield(*SingleGraphReader<GraphType>(entry.path()).read().begin());
            }
        }
    });
}

template class MultipleGraphsReader<DirectedGraph>;
template class MultipleGraphsReader<Automata>;
