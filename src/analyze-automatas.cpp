#include <filesystem>
#include <Filters/Predicates/Predicates.h>
#include <Filters/SimpleFilter/SimpleFilter.h>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include <Selectors/Selectors.h>

#include "types/types.h"

boost::coroutines2::coroutine<DirectedGraph&>::pull_type getGraphsToAnalyze() {
    std::cout << "Select how to pass graphs to analyzer [1]" << std::endl;
    std::cout << "1: From disk" << std::endl;
    std::cout << "2: From generator" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    int choice = 0;
    try {
        choice = std::stoi(option);
    } catch (...) {
        choice = -1; // Invalid input
    }

    if (choice == 2) {
        return GraphCoroutine::pull_type([](GraphCoroutine::push_type& yield) {
            auto generator = getGenerator<DirectedGraph>(
                [] { return new AllTwoOutgoingEdges<DirectedGraph>();},
                "AllTwoOutgoingEdges"
            );

            auto filter = SimpleFilter<DirectedGraph>(true);

            for (auto& graph : generator->generateGraphs()) {
                if (filter.isAccepted(graph)) {
                    yield(graph);
                }
            }
        });
    }
    auto reader = getReader<DirectedGraph>(
        [] {return new SingleGraphReader<DirectedGraph>();},
        "SingleGraphReader<Graph>()"
    );
    return reader->read();
}

struct Analytics {
    int synchronizedCount;
    int nonSynchronizedCount;
    int maxRepaintingStepsCount;
    int id;
};

int findLongestToGreen(Graph& g) {
    auto [vi, vi_end] = boost::vertices(g);

    std::vector<int> distance(boost::num_vertices(g), INT_MAX);

    std::queue<DirectedGraph::vertex_descriptor> q;

    for (auto it = vi; it != vi_end; ++it) {
        auto start = *it;

        // Пропускаем зелёные вершины
        if (g[start].fillcolor != "green") continue;

        distance[start] = 0;

        q.push(start);

        while (!q.empty()) {
            auto v = q.front();
            q.pop();

            auto [ai, ai_end] = boost::adjacent_vertices(v, g);
            for (auto it_adj = ai; it_adj != ai_end; ++it_adj) {
                auto u = *it_adj;

                if (g[u].fillcolor != "green" && distance[u] > distance[v] + 1) {
                    q.push(u);
                    distance[u] = distance[v] + 1;
                }
            }
        }
    }

    return *std::ranges::max_element(distance);
}

Analytics generateAutomatasGraph(DirectedGraph& g, IFilter<Automata>& filter, Graph& new_graph, int id) {
    auto n = g.m_vertices.size();
    std::map<int, DirectedGraph::vertex_descriptor> vertex_map;

    for (int mask = 0; mask < 1 << n; mask++) {
        auto v_new = boost::add_vertex(new_graph);
        vertex_map[mask] = v_new;
    }

    auto [vi, vi_end] = boost::vertices(new_graph);
    for (auto it = vi; it != vi_end; ++it) {
        new_graph[*it].node_id = *it;
    }

    auto generator = AutomatasFromGraph<AutomataGenerationResult>(g);

    int count = 0;
    for (auto [mask, automata_ptr]  : generator.generateGraphs()) {
        for (int i = 0; i < n; ++i) {
            unsigned int flipped = mask ^ (1 << i);
            auto [edge_it, exists] = boost::edge(vertex_map[mask], vertex_map[flipped], new_graph);
            if (!exists) {
                boost::add_edge(vertex_map[mask], vertex_map[flipped], new_graph);
            }
        }
        auto& automata = *automata_ptr;
        if (filter.isAccepted(automata)) {
            new_graph[vertex_map[mask]].fillcolor = "green";
            count++;
        }
    }

    const Analytics result {
        .synchronizedCount = count,
        .nonSynchronizedCount = (1 << n) - count,
        .maxRepaintingStepsCount = findLongestToGreen(new_graph),
        .id = id
    };
    return result;
}

int main() {
    auto graphs = getGraphsToAnalyze();

    auto recorder = getRecorder<Graph>(
        [] { return new DiskRecorder<Graph>("./automatas-graphs/");},
        "DiskRecorder(./automatas-graphs/)"
    );

    auto filter = SimpleFilter(false, std::vector<std::function<bool(Automata)>> {
        isSynchronized,
    });

    std::map<int, std::vector<int>> maxDistance;
    std::map<int, std::vector<int>> nonSyncVertexes;

    std::cout << "Limit of listed graphs for each category [20]:" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    int limit = 20;
    if (!option.empty()) {
        try {
            limit = std::stoi(option);
        } catch (...) {
            limit = -1; // Invalid input
        }
    }

    // Ask for filename
    std::cout << "Enter filename to write analytics [analytics.txt]:" << std::endl;
    std::string filename;
    std::getline(std::cin, filename);

    if (filename.empty()) {
        filename = "analytics.txt"; // default
    }

    // Try opening file
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        throw std::runtime_error("Could not open file for writing: " + filename);
    }

    int count = 0;
    for (auto graph : graphs) {
        Graph new_graph;
        auto info = generateAutomatasGraph(graph, filter, new_graph, count);

        if (maxDistance[info.maxRepaintingStepsCount].size() < limit) {
            maxDistance[info.maxRepaintingStepsCount].push_back(info.id);
            recorder->recordGraph(new_graph, std::to_string(info.id));
        }
        if (nonSyncVertexes[info.nonSynchronizedCount].size() < limit) {
            nonSyncVertexes[info.nonSynchronizedCount].push_back(info.id);
            recorder->recordGraph(new_graph, std::to_string(info.id));
        }

        if (++count % 1000 == 0) {
            std::cout << count << std::endl;
        }
    }

    auto printStats = [&outFile](const std::map<int, std::vector<int>>& data, const std::string& title) {
        for (const auto& [key, ids] : data) {
            std::cout << title << " " << key << ": " << ids.size() << std::endl;
            outFile << title << " " << key << ": " << ids.size() << "\n";
            for (int id : ids) {
                outFile << id << " ";
            }
            outFile << "\n";
        }
        outFile << "\n";
    };

    printStats(maxDistance, "MaxDistance statistics");
    printStats(nonSyncVertexes, "NonSyncVertexes statistics");

    outFile.close();

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}