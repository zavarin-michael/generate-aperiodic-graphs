#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>

#include "Generators/AutomatasFromGraph/AutomatasFromGraph.h"
#include "Recorders/Utils/Copy.h"
#include "Selectors/Selectors.h"
#include "types/types.h"
#include "Utils/Utils.h"

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
        g[*it].node_id = getVertexName(*it);
    }

    auto generator = AutomatasFromGraph<AutomataGenerationResult>(g, true);

    for (auto [mask, automata_ptr] : generator.generateGraphs()) {
        recorder->recordGraph(*automata_ptr, std::to_string(mask));
    }

    IRecorder<DirectedGraph>* copy = Copy<DirectedGraph, Automata>(recorder);
    copy->recordGraph(g, "graph");

    std::cout << "Press Enter to exit...";
    std::cin.get();

    return 0;
}