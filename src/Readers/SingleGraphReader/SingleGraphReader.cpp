#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include "SingleGraphReader.h"

#include <filesystem>

template<typename GraphType>
SingleGraphReader<GraphType>::SingleGraphReader(const std::filesystem::path& filepath) {
    auto localFilepath = filepath;
    if (filepath.empty()) {
        std::string path;
        std::cout << "\n"
                  << "+=====================================+\n"
                  << "|       INITIALIZATION OF READER      |\n"
                  << "+=====================================+\n\n";
        std::cout << std::left << "-> Enter filename to read: ";
        std::getline(std::cin, path);
        localFilepath = path;
    }

    this->file.open(localFilepath, std::ios::in);
    if (!this->file) {
        throw std::runtime_error("Cannot open DOT file: " + localFilepath.string());
    }
}


template<>
GraphCoroutine::pull_type SingleGraphReader<DirectedGraph>::read() {
    return GraphCoroutine::pull_type([this](GraphCoroutine::push_type& yield) {
        DirectedGraph g;
        boost::dynamic_properties dp(boost::ignore_other_properties);
        if (!boost::read_graphviz(file, g, dp)) {
            throw std::runtime_error("Failed to read DOT file");
        }
        yield(g);
    });
}

template<>
AutomataCoroutine::pull_type SingleGraphReader<Automata>::read() {
    return AutomataCoroutine::pull_type([this](AutomataCoroutine::push_type& yield) {
        Automata g;

        boost::dynamic_properties dp;
        dp.property("label", boost::get(&VertexProperties::node_id, g));
        dp.property("node_id", boost::get(&VertexProperties::node_id, g));
        dp.property("label", boost::get(&EdgeProperties::mark, g));

        if (!boost::read_graphviz(file, g, dp)) {
            throw std::runtime_error("Failed to read DOT file");
        }
        yield(g);
    });
}

template class SingleGraphReader<DirectedGraph>;
template class SingleGraphReader<Automata>;
