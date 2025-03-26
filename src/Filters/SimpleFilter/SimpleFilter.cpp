#include "SimpleFilter.h"
#include <types/types.h>

SimpleFilter::SimpleFilter(std::vector<std::function<bool(Graph)>> predicates) {
  this->predicates = predicates;
};

bool SimpleFilter::isAccepted(Graph graph) {
  for (auto filter : this->predicates) {
    if (!filter(graph)) return false;
  }
  return true;
}
