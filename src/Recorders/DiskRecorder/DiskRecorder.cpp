#include "DiskRecorder.h"
#include <filesystem>
#include <boost/graph/graphviz.hpp>

using namespace std;

void DiskRecorder::initialize() {
    cout << "|---------------------------|" << endl;
    cout << "|INITIALIZATION OF RECOREDER|" << endl;
    cout << "|---------------------------|" << endl << endl;
    cout << "Enter sub directory name:" << endl;
    cin >> subDirectory;

    const filesystem::path dirName(GRAPH_DIR);
    const filesystem::path dirPath = dirName / subDirectory;
    create_directories(dirPath);
}

void DiskRecorder::recordGraph(const Graph& g) {
    filesystem::path dirName(GRAPH_DIR);
    const auto filename = to_string(graphCount) + ".dot";
    filesystem::path filePath = dirName / subDirectory / filename;

    if (exists(filePath) && !isRewriteFiles) {
        return;
    }

    ofstream ofs(filePath, ios::trunc);
    if (!ofs) {
        throw runtime_error("Could not open file for writing: " + filePath.string());
    }

    boost::write_graphviz(ofs, g);
    graphCount++;
}