#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>

#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include "Recorders/Utils/Copy.h"
#include "Selectors/Selectors.h"
#include "types/types.h"

int main() {
    auto reader = SingleGraphReader<Graph>();
    // auto recorder = ConsoleRecorder<Automata>();
    auto recorder = getRecorder<Automata>(
        [] {return new DiskRecorder<Automata>("./automatas/");},
        "DiskRecorder(./automatas/)"
    );

    auto g = *reader.read().begin();
    auto generator = AutomatasFromGraph<AutomataGenerationResult>(g);

    for (auto [mask, automata_ptr] : generator.generateGraphs()) {
        recorder->recordGraph(*automata_ptr);
    }

    auto copy = *Copy<DiskRecorder<Graph>, Automata>(recorder);
    copy
    .setFilename("graph.dot")
    .recordGraph(g);
}