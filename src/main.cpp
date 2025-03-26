#include <iostream>
#include <vector>

#include "types/types.h"
#include <Generators/TwoOutgoingEdges/TwoOutgoingEdges.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Filters/SimpleFilter/SimpleFilter.h>
#include <Filters/Predicates/Predicates.h>

int main() {
    int n;
    std::cout << "Enter number of vertexes" << '\n';
    std::cin >> n;

    auto nString = std::to_string(n);
    auto recorder = DiskRecorder();
    auto generator = TwoOutgoingEdges(n);
    auto filter = SimpleFilter(std::vector<std::function<bool(Graph)>> {
        isStrongConnected,
        // isAperiodic,
        isNotAperiodic,
    });
    auto count = 0;

    for (auto graph : generator.generateGraphs()) {
        if (filter.isAccepted(graph)) {
            recorder.recordGraph(graph);
            count++;
        }
    }

    std::cout << count << '\n';
    return 0;
}
