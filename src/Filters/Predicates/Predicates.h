#ifndef PREDICATTES_H
#define PREDICATTES_H

#include <types/types.h>

template <typename GraphType>
bool isStrongConnected(const GraphType& graph);
template <typename GraphType>
bool isAperiodic(const GraphType& graph);
template <typename GraphType>
bool isNotAperiodic(const GraphType& graph);
bool isSynchronized(const Automata& automata);
bool isSynchronizedNew(const BisetGraph& graph, long long mask);

#include "Predicates.tpp"

#endif