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
    auto reader = SingleGraphReader<DirectedGraph>();
    // auto recorder = ConsoleRecorder<Automata>();
    auto recorder = getRecorder<Automata>(
        [] {return new DiskRecorder<Automata>("./automatas/");},
        "DiskRecorder(./automatas/)"
    );

    auto g = *reader.read().begin();

    auto [vi, vi_end] = boost::vertices(g);
    for (auto it = vi; it != vi_end; ++it) {
        g[*it].node_id = *it;
    }

    auto generator = AutomatasFromGraph<AutomataGenerationResult>(g);

    for (auto [mask, automata_ptr] : generator.generateGraphs()) {
        recorder->recordGraph(*automata_ptr);
    }

    IRecorder<DirectedGraph>* copy = Copy<DirectedGraph, Automata>(recorder);
    copy->recordGraph(g, "graph");

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}