#include "TwoOutgoingEdges.h"

#include "types/types.h"

TwoOutgoingEdges::TwoOutgoingEdges(const int vertexes_count) {
    this->vertexes_count = vertexes_count;
}

GraphCoroutine::pull_type TwoOutgoingEdges::generateGraphs() {
    auto n = vertexes_count;
    return GraphCoroutine::pull_type([n](GraphCoroutine::push_type& yield) {
        auto pair_vertices = std::vector<std::pair<int, int>>();

        for (auto i = 0; i < n; ++i) {
            for (auto j = i; j < n; ++j) {
                pair_vertices.push_back(std::make_pair(i, j));
            }
        }

        std::vector<size_t> indices(n, 0);

        bool done = false;

        while (true) {
            auto graph = Graph(n);

            for (int i = 0; i < n; ++i) {
                auto [v, u] = pair_vertices[indices[i]];

                add_edge(i, u, graph);
                add_edge(i, v, graph);
            }

            yield(graph);

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
    });
}

