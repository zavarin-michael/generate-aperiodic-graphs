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

    for (auto automata : makeAutomatasFromGraph(g)) {
        recorder.recordGraph(automata.second);
    }

    DiskRecorder<Graph>(recorder.dirPath, "./", false)
    .setFilename("graph.dot")
    .recordGraph(g);

    auto reader2 = SingleGraphReader<Automata>("./automatas/4/0.dot");
    auto automata = reader2.readGraph();

    DiskRecorder<Automata>(recorder.dirPath, "./", false)
    .setFilename("new_graph.dot")
    .recordGraph(automata);
}