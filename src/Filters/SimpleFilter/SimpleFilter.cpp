#include "SimpleFilter.h"
#include <types/types.h>

template <typename GraphType>
SimpleFilter<GraphType>::SimpleFilter(std::vector<std::function<bool(GraphType)>> predicates) {
  this->predicates = predicates;
};

template <typename GraphType>
bool SimpleFilter<GraphType>::isAccepted(GraphType& graph) {
  for (auto filter : this->predicates) {
    if (!filter(graph)) return false;
  }
  return true;
}

template class SimpleFilter<Graph>;
template class SimpleFilter<Automata>;