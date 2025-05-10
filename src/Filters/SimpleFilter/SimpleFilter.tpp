#include "SimpleFilter.h"
#include <iostream>
#include <Filters/Predicates/Predicates.h>
#include <types/types.h>

#include "Utils/Utils.h"

template <typename GraphType>
struct PredicateSet {
  std::unordered_map<int, std::function<std::function<bool(GraphType)>()>> map;
  std::unordered_map<int, std::string> descriptions;
};

template <typename GraphType>
PredicateSet<GraphType> getCommonPredicates() {
  return {
      {
        {1, [] { return [](GraphType g) { return isStrongConnected(g); }; }},
        {2, [] { return [](GraphType g) { return isAperiodic(g); }; }},
        {3, [] { return [](GraphType g) { return isNotAperiodic(g); }; }},
        {4, [] { return [](GraphType g) { return isEulerian(g); }; }},
        {5, [] {
          std::string input;
          size_t number = 1;
          std::cout << std::left << "-> Number of cycles [" << number << "]: ";
          std::getline(std::cin, input);
          if (!input.empty()) {
            number = parsePositiveInt(input, "Number of cycles");
          }

          return [number](GraphType g) { return isMatchLoopsCount(g, number); };
        }},
        {6, [] {
          std::string input;
          size_t number = 1;
          std::cout << std::left << "-> Number of multi edges [" << number << "]: ";
          std::getline(std::cin, input);
          if (!input.empty()) {
            number = parsePositiveInt(input, "Number of multi edges");
          }

          return [number](GraphType g) { return isMatchMultipleEdgesCount(g, number); };
        }},
      },
      {
        {1, "isStrongConnected"},
        {2, "isAperiodic"},
        {3, "isNotAperiodic"},
        {4, "isEulerian"},
        {5, "isMatchLoopsCount"},
        {6, "isMatchMultipleEdgesCount"},
      }
  };
}

template<typename GraphType>
PredicateSet<GraphType> getAvailablePredicates() {}

template <>
PredicateSet<DirectedGraph> getAvailablePredicates<DirectedGraph>() {
  return getCommonPredicates<DirectedGraph>();
}

template <>
PredicateSet<Automata> getAvailablePredicates<Automata>() {
  auto m = getCommonPredicates<Automata>();
  m.map[100] = [] {return isSynchronized;};
  m.descriptions[100] = "isSynchronized" ;
  return m;
}

template <typename GraphType>
SimpleFilter<GraphType>::SimpleFilter(const bool is_custom, std::vector<std::function<bool(GraphType)>> predicates) {
  if (!is_custom) {
    this->predicates = predicates;
    return;
  }

  auto allPredicates = getAvailablePredicates<GraphType>();

  std::cout << "Select filters (space-separated numbers):\n";
  for (const auto& [id, desc] : allPredicates.descriptions) {
    std::cout << id << ": " << desc << '\n';
  }

  const auto choices = getInput();

  for (const auto& choice : choices) {
    if (allPredicates.map.contains(choice)) {
      this->predicates.push_back(allPredicates.map.at(choice)());
    } else {
      std::cout << "Invalid filter: " << choice << ". Ignored!" << std::endl;
    }
  }
}

template <typename GraphType>
bool SimpleFilter<GraphType>::isAccepted(GraphType& graph) {
  for (auto filter : this->predicates) {
    if (!filter(graph)) return false;
  }
  return true;
}

template class SimpleFilter<DirectedGraph>;
template class SimpleFilter<Automata>;