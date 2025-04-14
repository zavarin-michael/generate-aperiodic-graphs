#include <filesystem>
#include <Filters/Predicates/Predicates.h>
#include <Filters/SimpleFilter/SimpleFilter.h>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include <Selectors/Selectors.h>

#include "types/types.h"

boost::coroutines2::coroutine<Graph&>::pull_type getGraphsToAnalyze() {
    std::cout << "Select how to pass graphs to analyzer [1]" << std::endl;
    std::cout << "1: from files" << std::endl;
    std::cout << "2: from generator select" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    int choice = 0;
    try {
        choice = std::stoi(option);
    } catch (...) {
        choice = -1; // Invalid input
    }

    switch (choice) {
        case 1:
        case 2:
            return GraphCoroutine::pull_type([](GraphCoroutine::push_type& yield) {
                auto generator = getGenerator<Graph>(
                    [] { return new AllTwoOutgoingEdges<Graph>();},
                    "AllTwoOutgoingEdges"
                );

                auto filter = SimpleFilter<Graph>(true);

                for (auto& graph : generator->generateGraphs()) {
                    if (filter.isAccepted(graph)) {
                        yield(graph);
                    }
                }
            });
        default:
            std::cout << "Invalid option. Defaulting to file select" << std::endl;
        auto reader = getReader<Graph>(
            [] {return new SingleGraphReader<Graph>("./automatas-graphs/");},
            "SingleGraphReader<Graph>()"
        );
        return reader->read();
    }
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

    std::queue<Graph::vertex_descriptor> q;

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

Analytics generateAutomatasGraph(Graph& g, IRecorder<Graph>& recorder, IFilter<Automata>& filter, int id) {
    auto n = g.m_vertices.size();
    Graph new_graph;
    std::map<int, Graph::vertex_descriptor> vertex_map;

    for (int mask = 0; mask < 1 << n; mask++) {
        auto v_new = boost::add_vertex(new_graph);
        vertex_map[mask] = v_new;
    }

    auto [vi, vi_end] = boost::vertices(new_graph);
    for (auto it = vi; it != vi_end; ++it) {
        new_graph[*it].node_id = *it;
    }

    auto generator = AutomatasFromGraph<AutomataGenerationResult>(g);

    size_t count = 0;
    for (auto [mask, automata_ptr]  : generator.generateGraphs()) {
        for (int i = 0; i < n; ++i) {
            unsigned int flipped = mask ^ (1 << i);
            boost::add_edge(vertex_map[mask], vertex_map[flipped], new_graph);
        }
        auto& automata = *automata_ptr;
        if (filter.isAccepted(automata)) {
            new_graph[vertex_map[mask]].fillcolor = "green";
            count++;
        }
    }

    recorder.recordGraph(new_graph);

    Analytics result;
    result.synchronizedCount = count;
    result.nonSynchronizedCount = (1 << n) - count;
    result.maxRepaintingStepsCount = findLongestToGreen(new_graph);
    result.id = id;
    return result;
}

int main() {
    // auto reader = SingleGraphReader<Automata>("./automatas/4/0.dot");
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

    int count = 0;
    for (auto graph : graphs) {
        auto info = generateAutomatasGraph(graph, *recorder, filter, count);
        if (!maxDistance.contains(info.maxRepaintingStepsCount)) {
            maxDistance[info.maxRepaintingStepsCount] = std::vector<int>();
        }
        maxDistance[info.maxRepaintingStepsCount].push_back(info.id);
        if (!nonSyncVertexes.contains(info.nonSynchronizedCount)) {
            nonSyncVertexes[info.nonSynchronizedCount] = std::vector<int>();
        }
        nonSyncVertexes[info.nonSynchronizedCount].push_back(info.id);
        count++;
        if (count % 1000 == 0) {
            std::cout << count << std::endl;
        }
    }

    for (auto i : maxDistance) {
        std::cout << "MaxDistance statistics " << i.first << ": " << i.second.size() << std::endl;
        for (auto it = i.second.begin(); it != i.second.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;
    for (auto i : maxDistance) {
        std::cout << "NonSyncVertexes statistics " << i.first << ": " << i.second.size() << std::endl;
        for (auto it = i.second.begin(); it != i.second.end(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }
}