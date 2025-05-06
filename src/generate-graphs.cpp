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
    const auto generator = getGenerator<DirectedGraph>(
        [] { return new RandomTwoOutgoingEdges<DirectedGraph>();},
        "RandomTwoOutgoingEdges"
    );

    auto const recorder = getRecorder<DirectedGraph>(
        [] { return new DiskRecorder<DirectedGraph>("./graphs/");},
        "DiskRecorder(./graphs/)"
    );
    // auto generator = RandomTwoOutgoingEdges();

    auto filter = SimpleFilter<DirectedGraph>(true);
    int c = 0;
    for (auto& graph : generator->generateGraphs()) {
        if (filter.isAccepted(graph)) {
            recorder->recordGraph(graph);
            c++;
        }
    }

    std::cout << "Graphs count: " << c << "\n";
    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}
