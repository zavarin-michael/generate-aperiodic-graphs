#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "Filters/IFilter.h"
#include "vector"

class SimpleFilter : public IFilter {
private:
  std::vector<std::function<bool(Graph)>> predicates;
public:
  bool isAccepted(Graph g) override;
  SimpleFilter(std::vector<std::function<bool(Graph)>> predicates);
};

#endif
