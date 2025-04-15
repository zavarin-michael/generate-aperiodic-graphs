#ifndef PREDICATTES_H
#define PREDICATTES_H

#include <types/types.h>

template <typename GraphType>
bool isStrongConnected(const GraphType& graph);
template <typename GraphType>
bool isAperiodic(const GraphType& graph);
template <typename GraphType>
bool isNotAperiodic(const GraphType& graph);
bool isSynchronized(const Automata& graph);
std::pair<DirectedGraph, std::set<DirectedGraph::vertex_descriptor>> getBisetGraph(const Automata& automata);

#include "Predicates.tpp"

#endif