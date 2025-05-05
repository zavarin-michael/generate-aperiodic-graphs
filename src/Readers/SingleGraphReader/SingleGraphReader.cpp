#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <fstream>
#include "SingleGraphReader.h"

#include <filesystem>

#include "Utils/Utils.h"

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
        DirectedGraph g_r, g;

        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("node_id", boost::get(&VertexProperties::node_id, g_r));

        if (!boost::read_graphviz(file, g_r, dp, "node_id")) {
            throw std::runtime_error("Failed to read DOT file");
        }

        std::map<DirectedGraph::vertex_descriptor, DirectedGraph::vertex_descriptor> vert_map;
        auto [vi, vi_end] = boost::vertices(g_r);
        for (auto it = vi; it != vi_end; ++it) {
            const auto new_v = add_vertex(g);
            g[new_v].node_id = getVertexName(new_v);
        }

        for (auto it = vi; it != vi_end; ++it) {
            vert_map[*it] = std::stoi(computeIntLabelFromNodeId(g_r[*it].node_id));
        }

        for (auto [ei, ei_end] = boost::edges(g_r); ei != ei_end; ++ei) {
            add_edge(vert_map[boost::source(*ei, g_r)], vert_map[boost::target(*ei, g_r)], g);
        }

        yield(g);
    });
}

template<>
AutomataCoroutine::pull_type SingleGraphReader<Automata>::read() {
    return AutomataCoroutine::pull_type([this](AutomataCoroutine::push_type& yield) {
        Automata g_r, g;

        boost::dynamic_properties dp(boost::ignore_other_properties);
        dp.property("node_id", boost::get(&VertexProperties::node_id, g_r));
        dp.property("label", boost::get(&AutomataEdgeProperties::mark, g_r));

        if (!boost::read_graphviz(file, g_r, dp, "node_id")) {
            throw std::runtime_error("Failed to read DOT file");
        }

        std::map<Automata::vertex_descriptor, Automata::vertex_descriptor> vert_map;
        auto [vi, vi_end] = boost::vertices(g_r);
        for (auto it = vi; it != vi_end; ++it) {
            const auto new_v = add_vertex(g);
            g[new_v].node_id = getVertexName(new_v);
        }

        for (auto it = vi; it != vi_end; ++it) {
            vert_map[*it] = std::stoi(computeIntLabelFromNodeId(g_r[*it].node_id));
        }

        for (auto [ei, ei_end] = boost::edges(g_r); ei != ei_end; ++ei) {
            AutomataEdgeProperties props;
            props.mark = g[*ei].mark;
            add_edge(vert_map[boost::source(*ei, g_r)], vert_map[boost::target(*ei, g_r)], props, g);
        }

        yield(g);
    });
}

template class SingleGraphReader<DirectedGraph>;
template class SingleGraphReader<Automata>;
