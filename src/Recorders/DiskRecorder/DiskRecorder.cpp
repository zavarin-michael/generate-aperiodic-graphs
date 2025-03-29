#include "DiskRecorder.h"
#include <filesystem>
#include <boost/graph/graphviz.hpp>
#include <types/types.h>

template<class GraphType>
const std::filesystem::path DiskRecorder<GraphType>::GRAPH_DIR{"./graphs/"};

template<class GraphType>
DiskRecorder<GraphType>::DiskRecorder() {
    std::cout << "\n"
              << "+=====================================+\n"
              << "|     INITIALIZATION OF RECOREDER     |\n"
              << "+=====================================+\n\n";
    std::cout << std::left << "-> Enter sub directory name: ";
    std::getline(std::cin, subDirectory);

    const std::filesystem::path dirPath = GRAPH_DIR / subDirectory;
    if (exists(dirPath)) {
        remove_all(dirPath);
    }
    create_directories(dirPath);
}

template<class GraphType>
void DiskRecorder<GraphType>::recordGraph(const GraphType& g) {
    const auto filename{std::to_string(graphCount) + ".dot"};
    std::filesystem::path filePath = GRAPH_DIR / subDirectory / filename;

    if (exists(filePath) && !isRewriteFiles) {
        return;
    }

    std::ofstream ofs(filePath, std::ios::trunc);
    if (!ofs) {
        throw std::runtime_error("Could not open file for writing: " + filePath.string());
    }

    write_graphviz(ofs, g);
    graphCount++;
}

template class DiskRecorder<Graph>;
template class DiskRecorder<Automata>;