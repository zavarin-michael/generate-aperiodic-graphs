#include <filesystem>
#include <Filters/Predicates/Predicates.h>
#include <Filters/SimpleFilter/SimpleFilter.h>

#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Generators/Functions/Functions.h>

#include "types/types.h"

int main() {
    // auto reader = SingleGraphReader<Automata>("./automatas/4/0.dot");
    auto reader = SingleGraphReader<Graph>();
    // auto recorder = ConsoleRecorder<Automata>();
    // auto recorder = DiskRecorder<Graph>("./analyzes/");
    auto graph = reader.readGraph();

    auto filter = SimpleFilter(std::vector<std::function<bool(Automata)>> {
        isSynchronized,
        // isNotAperiodic,
    });

    size_t count = 0;
    for (auto& automata : makeAutomatasFromGraph(graph)) {
        if (filter.isAccepted(automata.second)) {
            // recorder.recordGraph(graph);
            count++;
        }
    }

    // recorder.recordGraph(getBisetGraph(automata).first);

    std::cout << count << std::endl;
}