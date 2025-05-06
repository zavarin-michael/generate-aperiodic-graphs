#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>

#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include "Generators/BisetGraph/BisetGraph.h"
#include "Recorders/Utils/Copy.h"
#include "Selectors/Selectors.h"
#include "types/types.h"

void performAutomataLogic() {
    auto reader = SingleGraphReader<Automata>();
    auto graphs = reader.read();
    auto g = *graphs.begin();
    auto generator = BisetGraphGenerator(g);

    auto recorder = getRecorder<BisetGraph>(
        [] {return new DiskRecorder<BisetGraph>("./biset-graphs/");},
        "DiskRecorder(./biset-graphs/)"
    );

    for (auto& graph : generator.generateGraphs()) {
        recorder->recordGraph(graph, "biset-graph");
    }
}

void performGraphLogic() {
    auto reader = SingleGraphReader<DirectedGraph>();
    auto graphs = reader.read();
    auto g = *graphs.begin();

    auto recorder = getRecorder<BisetGraph>(
        [] {return new DiskRecorder<BisetGraph>("./biset-graphs/");},
        "DiskRecorder(./biset-graphs/)"
    );

    auto automatas_generator = AutomatasFromGraph<AutomataGenerationResult>(g, true);
    for (auto [mask, automata_ptr] : automatas_generator.generateGraphs()) {
        auto generator = BisetGraphGenerator(*automata_ptr);
        for (auto& graph : generator.generateGraphs()) {
            recorder->recordGraph(graph, std::to_string(mask));
        }
    }
}

int main() {
    std::cout << "Generate default biset[automata]:" << std::endl;
    std::cout << "1: from automata\n";
    std::cout << "2: from graph\n";

    std::string option;
    std::getline(std::cin, option);

    if (option.empty() || option == "1") {
        performAutomataLogic();
    } else {
        performGraphLogic();
    }

    std::cout << "Press Enter to exit...";
    std::cin.get();
    return 0;
}