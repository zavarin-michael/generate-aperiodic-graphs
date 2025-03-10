#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graphviz.hpp>
#include <filesystem>
#include <fstream>
#include <stdexcept>

using namespace boost;
using namespace std;

typedef adjacency_list<vecS, vecS, directedS> Graph;
const string GRAPH_DIR = "graphs";

void print_graph(const Graph& g, int n) {
    cout << "Graph:" << endl;
    for (int v = 0; v < n; ++v) {
        cout << v << ": ";
        // Получаем диапазон исходящих рёбер для вершины v.
        auto edges = out_edges(v, g);
        for (auto it = edges.first; it != edges.second; ++it) {
            int target_vertex = target(*it, g);
            cout << target_vertex << " ";
        }
        cout << endl;
    }
    cout << "------------------------" << endl;
}

template<typename GraphType>
void writeGraphToFile(const GraphType& g, const std::string& filename, const std::string& dirname) {
    filesystem::path dirPath(GRAPH_DIR);
    filesystem::path filePath = dirPath / dirname / filename;

    std::ofstream ofs(filePath);
    if (!ofs) {
        throw std::runtime_error("Did not found file to write: " + filename);
    }
    boost::write_graphviz(ofs, g);
}

void initializeDirs(const std::string& dirname) {
    filesystem::path dirPath(GRAPH_DIR);

    if (!exists(dirPath)) {
        if (!create_directory(dirPath)) {
            throw std::runtime_error("GRAPH_DIR dir error");
        } else {
            cout << "Dir GRAPH_DIR created successfully." << endl;
        }
    }

    filesystem::path filePath = dirPath / dirname;

    if (!exists(filePath)) {
        cout << filePath;
        if (!create_directory(filePath)) {
            throw std::runtime_error("Did not found dir: " + filePath.string());
        } else {
            cout << "Dir '" << filePath.string() << "' created successfully." << endl;
        }
    }
}

int main() {
    int n = 3;
    std::cout << "Enter number of vertexes" << std::endl;
    // std::cin >> n;

    auto nString = to_string(n);
    initializeDirs(nString);

    // typedef graph_traits<Graph>::vertex_descriptor Vertex;
    // auto index = get(vertex_index, graph);
    auto pair_vertices = vector<pair<int, int>>();

    for (auto i = 0; i < n; ++i) {
        for (auto j = i; j < n; ++j) {
            pair_vertices.push_back(make_pair(i, j));
        }
    }

    cout << pair_vertices.size() << endl;

    vector<size_t> indices(n, 0);

    bool done = false;
    auto count = 0;
    while (true) {
        auto graph = Graph(n);

        for (int i = 0; i < n; ++i) {
            auto [v, u] = pair_vertices[indices[i]];
            add_edge(i, u, graph);
            add_edge(i, v, graph);
        }

        // print_graph(graph, n);
        try {
            writeGraphToFile(graph, to_string(count) + ".dot", nString);
        } catch (const std::exception& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

        count++;

        for (int i = n - 1; i >= 0; --i) {
            if (indices[i] < pair_vertices.size() - 1) {
                indices[i]++;
                for (int j = i + 1; j < n; ++j) {
                    indices[j] = 0;
                }
                break;
            }
            if (i == 0) {
                done = true;
            }
        }

        if (done) {
            break;
        }
    }

    cout << count << endl;
    return 0;
}
