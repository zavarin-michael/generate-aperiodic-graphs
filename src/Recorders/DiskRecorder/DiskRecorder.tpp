#include "DiskRecorder.h"
#include <filesystem>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <utility>
#include <types/types.h>

#include "Utils/Utils.h"


template<class GraphType>
struct LabelFromNodeIdMap {
    using key_type = typename boost::graph_traits<GraphType>::vertex_descriptor;
    using value_type = std::string;
    using reference = const std::string;
    using category = boost::readable_property_map_tag;

    LabelFromNodeIdMap(const GraphType& g) : graph(g) {}

    reference operator[](key_type v) const {
        return computeIntLabelFromNodeId(graph[v].node_id);
    }
private:
    const GraphType& graph;
};

template <typename Graph>
std::string get(const LabelFromNodeIdMap<Graph>& map, typename boost::graph_traits<Graph>::vertex_descriptor v) {
    return map[v];
}


template<class GraphType>
DiskRecorder<GraphType>::DiskRecorder(std::filesystem::path rootDir, std::filesystem::path subDirs, bool isRewriteFiles) {
    this->isRewriteFiles = isRewriteFiles;

    if (rootDir.empty() || subDirs.empty()) {
        std::cout << "+=====================================+\n"
                  << "|     INITIALIZATION OF RECOREDER     |\n"
                  << "+=====================================+\n\n";
    }

    if (rootDir.empty()) {
        std::string rootDirStr;
        std::cout << std::left << "-> Enter root directory name: ";
        std::getline(std::cin, rootDirStr);
        rootDir = std::filesystem::path(rootDirStr);
    }

    if (subDirs.empty()) {
        std::string subDirStr;
        std::cout << std::left << "-> Enter sub directory name: ";
        std::getline(std::cin, subDirStr);
        subDirs = std::filesystem::path(subDirStr);
    }

    dirPath = rootDir / subDirs;
    if (exists(dirPath) && isRewriteFiles) {
        remove_all(dirPath);
    }
    create_directories(dirPath);
}

template<>
inline void DiskRecorder<UndirectedGraph>::writeGraph(std::ofstream &ofs, UndirectedGraph& g) {
    boost::dynamic_properties dp;
    dp.property("fillcolor", get(&VertexProperties::fillcolor, g));
    dp.property("node_id", get(&VertexProperties::node_id, g));
    dp.property("style",
        boost::make_constant_property<UndirectedGraph::vertex_descriptor>(std::string("filled"))
    );
    LabelFromNodeIdMap label_map(g);
    dp.property("label", label_map);

    // add option to editor if needed
    // ranksep=1.5;
    // nodesep=1.0;
    // rankdir=BT;
    write_graphviz_dp(ofs, g, dp);
}

template<>
inline void DiskRecorder<DirectedGraph>::writeGraph(std::ofstream &ofs, DirectedGraph& g) {
    boost::dynamic_properties dp;
    dp.property("fillcolor", get(&VertexProperties::fillcolor, g));
    dp.property("node_id", get(&VertexProperties::node_id, g));
    dp.property("style",
        boost::make_constant_property<DirectedGraph::vertex_descriptor>(std::string("filled"))
    );
    LabelFromNodeIdMap label_map(g);
    dp.property("label", label_map);

    write_graphviz_dp(ofs, g, dp);
}

template<>
inline void DiskRecorder<BisetGraph>::writeGraph(std::ofstream &ofs, BisetGraph& g) {
    boost::dynamic_properties dp;
    dp.property("fillcolor", get(&BisetVertexProperties::fillcolor, g));
    dp.property("node_id", get(&BisetVertexProperties::node_id, g));
    dp.property("style",
        boost::make_constant_property<DirectedGraph::vertex_descriptor>(std::string("filled"))
    );
    dp.property("label", get(&BisetGraphProperties::meta, g));

    write_graphviz_dp(ofs, g, dp);
}

template<>
inline void DiskRecorder<Automata>::writeGraph(std::ofstream &ofs, Automata& g) {
    boost::dynamic_properties dp;
    dp.property("node_id", get(&VertexProperties::node_id, g));
    dp.property("fillcolor", get(&VertexProperties::fillcolor, g));
    dp.property("style",
        boost::make_constant_property<DirectedGraph::vertex_descriptor>(std::string("filled"))
    );
    dp.property("label", get(&AutomataEdgeProperties::mark, g));
    LabelFromNodeIdMap label_map(g);
    dp.property("label", label_map);

    write_graphviz_dp(ofs, g, dp);
}

template<class GraphType>
void DiskRecorder<GraphType>::recordGraph(GraphType& g, std::string name) {
    std::string filename;
    if (this->filename.empty()) {
        filename = (name.empty() ? std::to_string(graphCount) : name) + ".dot";
    } else {
        filename = this->filename;
    }

    const std::filesystem::path filePath = dirPath / filename;

    if (exists(filePath)) {
        return;
    }

    std::ofstream ofs(filePath, std::ios::trunc);
    if (!ofs) {
        throw std::runtime_error("Could not open file for writing: " + filePath.string());
    }

    this->writeGraph(ofs, g);
    graphCount++;
}

template<class GraphType>
DiskRecorder<GraphType> DiskRecorder<GraphType>::setFilename(const std::string& filename) {
    this->filename=filename;
    return *this;
}

template<class GraphType>
DiskRecorder<GraphType> DiskRecorder<GraphType>::setDirPath(std::filesystem::path dirPath) {
    this->dirPath = std::move(dirPath);
    return *this;
}

template class DiskRecorder<DirectedGraph>;
template class DiskRecorder<Automata>;
template class DiskRecorder<UndirectedGraph>;
template class DiskRecorder<BisetGraph>;