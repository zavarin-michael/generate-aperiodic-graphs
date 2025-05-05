#include <filesystem>
#include <Filters/Predicates/Predicates.h>
#include <Filters/SimpleFilter/SimpleFilter.h>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include <Selectors/Selectors.h>

#include "Recorders/Utils/Copy.h"
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
    long long synchronizedCount;
    long long nonSynchronizedCount;
    long long maxRepaintingStepsCount;
    long long vertexMaxRepaintingStepsCount;
    long long id;
};

void getMinDistancesToGreen(UndirectedGraph& g, std::vector<std::pair<int, int>>& distance) {
    auto [vi, vi_end] = boost::vertices(g);

    std::queue<DirectedGraph::vertex_descriptor> q;
    distance.resize(vi_end - vi);

    for (auto it = vi; it != vi_end; ++it) {
        distance[*it] = std::make_pair(INT_MAX, *it);
    }

    for (auto it = vi; it != vi_end; ++it) {
        auto start = *it;

        // Пропускаем зелёные вершины
        if (g[start].fillcolor != "green") continue;

        distance[start].first = 0;

        q.push(start);

        while (!q.empty()) {
            auto v = q.front();
            q.pop();

            auto [ai, ai_end] = boost::adjacent_vertices(v, g);
            for (auto it_adj = ai; it_adj != ai_end; ++it_adj) {
                auto u = *it_adj;

                if (g[u].fillcolor != "green" && distance[u].first > distance[v].first + 1) {
                    q.push(u);
                    distance[u].first = distance[v].first + 1;
                }
            }
        }
    }
}

std::pair<int, int> findLongestToGreen(UndirectedGraph& g) {
    std::vector<std::pair<int, int>> distances;

    getMinDistancesToGreen(g, distances);

    return *std::ranges::max_element(distances, [](const auto& a, const auto& b) {
        return a.first < b.first;
    });
}

Analytics generateAutomatasGraph(DirectedGraph& g, IFilter<Automata>& filter, UndirectedGraph& new_graph, int id, unsigned long long xorMask) {
    auto n = g.m_vertices.size() - 1;
    std::map<int, DirectedGraph::vertex_descriptor> vertex_map;

    for (int mask = 0; mask < 1 << n; mask++) {
        auto v_new = boost::add_vertex(new_graph);
        vertex_map[mask] = v_new;
    }

    auto [vi, vi_end] = boost::vertices(new_graph);
    for (auto it = vi; it != vi_end; ++it) {
        new_graph[*it].node_id = getVertexName(*it);
    }
    long long count = 0;

    // auto generator = AutomatasFromGraph<AutomataGenerationResult>(g, false, true);
    // for (auto [mask, automata_ptr]  : generator.generateGraphs()) {
    //     auto newMask = xorMask ^ mask;
    //     for (int i = 0; i < n; ++i) {
    //         unsigned int flipped = newMask ^ (1 << i);
    //         auto [edge_it, exists] = boost::edge(vertex_map[newMask], vertex_map[flipped], new_graph);
    //         if (!exists) {
    //             boost::add_edge(vertex_map[newMask], vertex_map[flipped], new_graph);
    //         }
    //     }
    //     auto& automata = *automata_ptr;
    //     if (filter.isAccepted(automata)) {
    //         new_graph[vertex_map[mask]].fillcolor = "green";
    //         count+=2;
    //     }
    // }

    auto bisetGraph = *BisetGraphGenerator(g).generateGraphs().begin();
    for (int mask = 0; mask < 1 << n; mask++) {
        long long newMask = xorMask ^ mask;
        for (int i = 0; i < n; ++i) {
            unsigned int flipped = newMask ^ (1 << i);
            auto [edge_it, exists] = boost::edge(vertex_map[newMask], vertex_map[flipped], new_graph);
            if (!exists) {
                boost::add_edge(vertex_map[newMask], vertex_map[flipped], new_graph);
            }
        }
        if (isSynchronizedNew(bisetGraph, newMask)) {
            new_graph[vertex_map[newMask]].fillcolor = "green";
            count+=2;
        }
    }

    auto [fst, snd] = findLongestToGreen(new_graph);

    const Analytics result {
        .synchronizedCount = count,
        .nonSynchronizedCount = (1 << n + 1) - count,
        .maxRepaintingStepsCount = fst,
        .vertexMaxRepaintingStepsCount = snd,
        .id = id
    };
    return result;
}

int main() {
    auto graphs = getGraphsToAnalyze();

    auto recorder = getRecorder<UndirectedGraph>(
        [] { return new DiskRecorder<UndirectedGraph>("./automatas-graphs/");},
        "DiskRecorder(./automatas-graphs/)"
    );

    auto copy = Copy<DirectedGraph, UndirectedGraph>(recorder);

    auto filter = SimpleFilter(false, std::vector<std::function<bool(Automata)>> {
        isSynchronized,
    });

    std::cout << "Limit of listed examples graphs for each category [20]:" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    int limit = 20;
    if (!option.empty()) {
        try {
            limit = std::stoi(option);
        } catch (...) {
            throw std::invalid_argument("Invalid option");
        }
    }

    // Ask for xor mask
    std::cout << "Enter xor mask for automatas graph [0]:" << std::endl;
    std::getline(std::cin, option);

    long long xorMask = 0;
    if (!option.empty()) {
        try {
            xorMask = std::stoi(option);
        } catch (...) {
            throw std::invalid_argument("Invalid option");
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

    std::map<long long, std::vector<int>> maxDistanceExamples;
    std::map<long long, std::vector<int>> nonSyncVertexesExamples;
    std::map<int, int> maxDistanceVertex;

    std::map<long long, long long> maxDistance;
    std::map<long long, long long> nonSyncVertexes;

    int count = 0;
    for (auto graph : graphs) {
        UndirectedGraph new_graph;
        auto info = generateAutomatasGraph(graph, filter, new_graph, count, xorMask);

        maxDistance[info.maxRepaintingStepsCount]++;
        nonSyncVertexes[info.nonSynchronizedCount]++;

        if (maxDistanceExamples[info.maxRepaintingStepsCount].size() < limit) {
            maxDistanceExamples[info.maxRepaintingStepsCount].push_back(info.id);
            maxDistanceVertex[info.id] = info.vertexMaxRepaintingStepsCount;
            recorder->recordGraph(new_graph, std::to_string(info.id));
            copy->recordGraph(graph, "graph" + std::to_string(info.id));
        }
        if (nonSyncVertexesExamples[info.nonSynchronizedCount].size() < limit) {
            nonSyncVertexesExamples[info.nonSynchronizedCount].push_back(info.id);
            maxDistanceVertex[info.id] = info.vertexMaxRepaintingStepsCount;
            recorder->recordGraph(new_graph, std::to_string(info.id));
            copy->recordGraph(graph, "graph" + std::to_string(info.id));
        }

        if (++count % 100 == 0) {
            std::cout << count << std::endl;
        }
    }

    auto printStats = [&outFile, &maxDistanceVertex](const std::map<long long, std::vector<int>>& examples, const std::map<long long, long long>& data, const std::string& title) {
        for (const auto& [key, count] : data) {
            std::cout << title << " " << key << ": " << count << std::endl;
            outFile << title << " " << key << ": " << count << "\n";
            for (int id : examples.at(key)) {
                outFile << id << "[" << maxDistanceVertex.at(id) << "] ";
            }
            outFile << "\n";
        }
        outFile << "\n";
    };

    printStats(maxDistanceExamples, maxDistance, "MaxDistance statistics");
    printStats(nonSyncVertexesExamples, nonSyncVertexes, "NonSyncVertexes statistics");

    outFile.close();

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}