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
    auto const recorder = getRecorder<Graph>(
        [] { return new DiskRecorder<Graph>("./graphs/");},
        "DiskRecorder(./graphs/)"
    );
    // auto generator = RandomTwoOutgoingEdges();
    const auto generator = getGenerator<Graph>(
        [] { return new AllTwoOutgoingEdges<Graph>();},
        "AllTwoOutgoingEdges"
    );

    auto filter = SimpleFilter<Graph>(true);
    auto count = 0;

    for (auto& graph : generator->generateGraphs()) {
        if (filter.isAccepted(graph)) {
            recorder->recordGraph(graph);
            count++;
        }
    }

    std::cout << count << '\n';
    return 0;
}
