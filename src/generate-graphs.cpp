#include <iostream>
#include <vector>

#include "types/types.h"
#include <Generators/AllTwoOutgoingEdges/AllTwoOutgoingEdges.h>
#include <Generators/RandomTwoOutgoingEdges/RandomTwoOutgoingEdges.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/NoRecorder/NoRecorder.h>
#include <Filters/SimpleFilter/SimpleFilter.h>
#include <Filters/Predicates/Predicates.h>

int main() {
    auto recorder = DiskRecorder<Graph>("./graphs/");
    auto generator = RandomTwoOutgoingEdges();
    auto filter = SimpleFilter(std::vector<std::function<bool(Graph)>> {
        isStrongConnected,
        isAperiodic,
        // isNotAperiodic,
    });
    auto count = 0;

    for (const auto& graph : generator.generateGraphs()) {
        if (filter.isAccepted(graph)) {
            recorder.recordGraph(graph);
            count++;
        }
    }

    std::cout << count << '\n';
    return 0;
}
