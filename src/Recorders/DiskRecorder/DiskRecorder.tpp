#include "DiskRecorder.h"
#include <filesystem>
#include <iostream>
#include <boost/graph/graphviz.hpp>
#include <utility>
#include <types/types.h>

template<class GraphType>
DiskRecorder<GraphType>::DiskRecorder(const std::filesystem::path &rootDir, std::filesystem::path subDirs, bool isRewriteFiles) {
    this->isRewriteFiles = isRewriteFiles;
    if (subDirs.empty()) {
        std::string subDirStr;
        std::cout << "\n"
                  << "+=====================================+\n"
                  << "|     INITIALIZATION OF RECOREDER     |\n"
                  << "+=====================================+\n\n";
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
inline void DiskRecorder<Graph>::writeGraph(std::ofstream &ofs, const Graph& g) {
    write_graphviz(ofs, g,
        boost::make_label_writer(boost::get(&VertexProperties::node_id, g)));
}

template<>
inline void DiskRecorder<Automata>::writeGraph(std::ofstream &ofs, const Automata& g) {
    write_graphviz(ofs, g,
    boost::make_label_writer(boost::get(&VertexProperties::node_id, g)),
        boost::make_label_writer(boost::get(&EdgeProperties::mark, g)));
}

template<class GraphType>
void DiskRecorder<GraphType>::recordGraph(const GraphType& g) {
    std::string filename;
    if (this->filename.empty()) {
        filename = std::to_string(graphCount) + ".dot";
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

template class DiskRecorder<Graph>;
template class DiskRecorder<Automata>;