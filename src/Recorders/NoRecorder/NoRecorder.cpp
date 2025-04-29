#include "NoRecorder.h"

#include <types/types.h>

template<class GraphType>
void NoRecorder<GraphType>::recordGraph(GraphType& g, std::string name) {
    return;
}

template class NoRecorder<DirectedGraph>;
template class NoRecorder<Automata>;
template class NoRecorder<UndirectedGraph>;
template class NoRecorder<BisetGraph>;