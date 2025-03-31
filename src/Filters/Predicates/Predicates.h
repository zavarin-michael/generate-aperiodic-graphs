#ifndef PREDICATTES_H
#define PREDICATTES_H

#include <types/types.h>

bool isStrongConnected(const Graph& graph);
bool isAperiodic(const Graph& graph);
bool isNotAperiodic(const Graph& graph);
bool isSynchronized(const Automata& graph);
std::pair<Graph, std::set<Graph::vertex_descriptor>> getBisetGraph(const Automata& automata);

#endif