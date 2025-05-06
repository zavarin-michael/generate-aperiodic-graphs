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

#include "Selectors/Selectors.h"

int main() {
    auto const recorder = getRecorder<DirectedGraph>(
        [] { return new DiskRecorder<DirectedGraph>("./graphs/");},
        "DiskRecorder(./graphs/)"
    );
    // auto generator = RandomTwoOutgoingEdges();
    const auto generator = getGenerator<DirectedGraph>(
        [] { return new AllTwoOutgoingEdges<DirectedGraph>();},
        "AllTwoOutgoingEdges"
    );

    auto filter = SimpleFilter<DirectedGraph>(true);

    for (auto& graph : generator->generateGraphs()) {
        if (filter.isAccepted(graph)) {
            recorder->recordGraph(graph);
        }
    }

    std::cout << "Graphs count: " << generator->countGeneratedGraphs() << "\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
