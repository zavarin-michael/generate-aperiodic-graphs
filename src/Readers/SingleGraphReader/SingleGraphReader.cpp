#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include "SingleGraphReader.h"

#include <filesystem>

template<typename GraphType>
SingleGraphReader<GraphType>::SingleGraphReader(std::filesystem::path filepath) {
    if (filepath.empty()) {
        std::string path;
        std::cout << "\n"
                  << "+=====================================+\n"
                  << "|       INITIALIZATION OF READER      |\n"
                  << "+=====================================+\n\n";
        std::cout << std::left << "-> Enter filename to read: ";
        std::getline(std::cin, path);
        filepath = path;
    }

    this->file.open(filepath, std::ios::in);
    if (!this->file) {
        throw std::runtime_error("Cannot open DOT file: " + filepath.string());
    }
}


template<>
Graph SingleGraphReader<Graph>::readGraph() {
    Graph g;
    boost::dynamic_properties dp(boost::ignore_other_properties);
    if (!boost::read_graphviz(file, g, dp)) {
        throw std::runtime_error("Failed to read DOT file");
    }
    return g;
}

template<>
Automata SingleGraphReader<Automata>::readGraph() {
    Automata g;

    boost::dynamic_properties dp;
    dp.property("fillcolor", boost::get(&VertexProperties::node_id, g));
    dp.property("node_id", boost::get(&VertexProperties::node_id, g));
    dp.property("label", boost::get(&EdgeProperties::mark, g));

    if (!boost::read_graphviz(file, g, dp)) {
        throw std::runtime_error("Failed to read DOT file");
    }
    return g;
}

template class SingleGraphReader<Graph>;
template class SingleGraphReader<Automata>;
