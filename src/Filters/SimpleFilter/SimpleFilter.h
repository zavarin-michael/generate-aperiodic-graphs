#ifndef SIMPLEFILTER_H
#define SIMPLEFILTER_H

#include "Filters/IFilter.h"
#include "vector"

template <typename GraphType>
class SimpleFilter : public IFilter<GraphType> {
private:
  std::vector<std::function<bool(GraphType)>> predicates;
  static std::vector<int> getInput();
public:
  bool isAccepted(GraphType& g) override;
  SimpleFilter(bool is_custom = false,  std::vector<std::function<bool(GraphType)>> predicates = {});
};

#include "SimpleFilter.tpp"

#endif
