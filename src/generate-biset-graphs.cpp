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

    auto recorder = getRecorder<DirectedGraph>(
        [] {return new DiskRecorder<DirectedGraph>("./biset-graphs/");},
        "DiskRecorder(./biset-graphs/)"
    );

    // Ask for xor mask
    std::cout << "Generate default biset[automata]:" << std::endl;
    std::cout << "1: from automata\n";
    std::cout << "2: from graph\n";

    std::string option;
    std::getline(std::cin, option);

    if (option.empty() || option == "1") {
        auto reader = SingleGraphReader<Automata>();
        auto graphs = reader.read();
        auto g = *graphs.begin();
        auto generator = BisetGraph(g);

        for (auto& graph : generator.generateGraphs()) {
            recorder->recordGraph(graph, "biset-automata");
        }
    } else {
        auto reader = SingleGraphReader<DirectedGraph>();
        auto graphs = reader.read();
        auto g = *graphs.begin();
        auto generator = BisetGraph(g);

        for (auto& graph : generator.generateGraphs()) {
            recorder->recordGraph(graph, "biset-graph");
        }
    }

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}