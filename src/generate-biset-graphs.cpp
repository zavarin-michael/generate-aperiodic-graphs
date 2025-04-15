#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>

#include "Generators/BisetGraph/BisetGraph.h"
#include "Recorders/Utils/Copy.h"
#include "Selectors/Selectors.h"
#include "types/types.h"

int main() {
    auto reader = SingleGraphReader<Automata>();
    auto recorder = getRecorder<DirectedGraph>(
        [] {return new DiskRecorder<DirectedGraph>("./biset-graphs/");},
        "DiskRecorder(./biset-graphs/)"
    );

    auto graphs = reader.read();
    auto g = *graphs.begin();
    auto generator = BisetGraph<DirectedGraph>(g);

    for (auto& graph : generator.generateGraphs()) {
        recorder->recordGraph(graph, "biset");
    }
}