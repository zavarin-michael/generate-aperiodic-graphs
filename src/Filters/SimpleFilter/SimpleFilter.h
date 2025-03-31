#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "Filters/IFilter.h"
#include "vector"

template <typename GraphType>
class SimpleFilter : public IFilter<GraphType> {
private:
  std::vector<std::function<bool(GraphType)>> predicates;
public:
  bool isAccepted(GraphType& g) override;
  SimpleFilter(std::vector<std::function<bool(GraphType)>> predicates);
};

#endif
