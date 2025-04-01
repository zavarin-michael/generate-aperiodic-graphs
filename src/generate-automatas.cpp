#include <filesystem>
#include <iostream>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Generators/Functions/Functions.h>

#include "types/types.h"

int main() {
    auto reader = SingleGraphReader<Graph>();
    // auto recorder = ConsoleRecorder<Automata>();
    auto recorder = DiskRecorder<Automata>("./automatas/");
    auto g = reader.readGraph();

    for (auto [mask, automata_ptr] : makeAutomatasFromGraph(g)) {
        auto& automata = *automata_ptr;
        recorder.recordGraph(automata);
    }

    DiskRecorder<Graph>(recorder.dirPath, "./", false)
    .setFilename("graph.dot")
    .recordGraph(g);
}