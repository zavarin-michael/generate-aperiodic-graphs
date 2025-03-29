#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include "SingleGraphReader.h"

#include <filesystem>

Graph SingleGraphReader::readGraph(const std::filesystem::path& path) {
    Graph g;
    std::ifstream file(path, std::ios::in);
    if (!file) {
        throw std::runtime_error("Cannot open DOT file: " + path.string());
    }

    boost::dynamic_properties dp(boost::ignore_other_properties);

    if (!boost::read_graphviz(file, g, dp)) {
        throw std::runtime_error("Failed to read DOT file: " + path.string());
    }

    return g;
}

