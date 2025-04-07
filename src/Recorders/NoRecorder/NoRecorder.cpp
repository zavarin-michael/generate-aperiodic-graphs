#include "NoRecorder.h"

#include <types/types.h>

template<class GraphType>
void NoRecorder<GraphType>::recordGraph(GraphType& g) {
    return;
}

template class NoRecorder<Graph>;
template class NoRecorder<Automata>;