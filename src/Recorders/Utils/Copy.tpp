#include <Recorders/IRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/NoRecorder/NoRecorder.h>
#include <types/types.h>

template<typename To, typename From>
IRecorder<To>* Copy(IRecorder<From>* from) {
    auto diskRecorder = dynamic_cast<const DiskRecorder<From>*>(from);
    if (diskRecorder) {
        return new DiskRecorder<To>(
            diskRecorder->dirPath,
            "./", // or diskRecorder->getSubDir() if you have it
            false
        );
    }

    auto consoleRecorder = dynamic_cast<const ConsoleRecorder<From>*>(from);
    if (consoleRecorder) {
        return new ConsoleRecorder<To>();
    }

    return new NoRecorder<To>();
}

template IRecorder<DirectedGraph>* Copy(IRecorder<DirectedGraph>* from);
template IRecorder<DirectedGraph>* Copy(IRecorder<Automata>* from);
template IRecorder<DirectedGraph>* Copy(IRecorder<Graph>* from);
template IRecorder<Automata>* Copy(IRecorder<DirectedGraph>* from);
template IRecorder<Automata>* Copy(IRecorder<Automata>* from);
template IRecorder<Automata>* Copy(IRecorder<Graph>* from);
template IRecorder<Graph>* Copy(IRecorder<DirectedGraph>* from);
template IRecorder<Graph>* Copy(IRecorder<Automata>* from);
template IRecorder<Graph>* Copy(IRecorder<Graph>* from);
