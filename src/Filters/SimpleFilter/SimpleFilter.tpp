#include "SimpleFilter.h"
#include <iostream>
#include <Filters/Predicates/Predicates.h>
#include <types/types.h>

template <typename GraphType>
std::vector<int> SimpleFilter<GraphType>::getInput() {
  std::string input;
  std::getline(std::cin, input);
  std::istringstream iss(input);

  std::vector<int> choices;
  int num;
  while (iss >> num) {
    choices.push_back(num);
  }

  return choices;
}

template <>
SimpleFilter<DirectedGraph>::SimpleFilter(const bool is_custom, std::vector<std::function<bool(DirectedGraph)>> predicates) {
  if (!is_custom) {
    this->predicates = predicates;
    return;
  }

  std::cout << "Select filters (space-separated numbers):\n";
  std::cout << "1: isStrongConnected\n";
  std::cout << "2: isAperiodic\n";
  std::cout << "3: isNotAperiodic\n";

  const auto choices = getInput();

  std::map<int, std::function<bool(DirectedGraph)>> allPredicates = {
    {1, [](DirectedGraph g) { return isStrongConnected(g); }},
    {2, [](DirectedGraph g) { return isAperiodic(g); }},
    {3, [](DirectedGraph g) { return isNotAperiodic(g); }},
  };


  for (const auto& choice : choices) {
    if (choice > 0 && choice < 4) {
      this->predicates.push_back(allPredicates[choice]);
    } else {
      std::cout << "Invalid filter:" << choice << ". Ignored!" << std::endl;
    }
  }
};

template <>
SimpleFilter<Automata>::SimpleFilter(const bool is_custom, std::vector<std::function<bool(Automata)>> predicates) {
  if (!is_custom) {
    this->predicates = predicates;
    return;
  }

  std::cout << "Select filters (space-separated numbers):\n";
  std::cout << "1: isStrongConnected\n";
  std::cout << "2: isAperiodic\n";
  std::cout << "3: isNotAperiodic\n";

  const auto choices = getInput();

  std::map<int, std::function<bool(Automata)>> allPredicates = {
    {1, [](Automata g) { return isStrongConnected(g); }},
    {2, [](Automata g) { return isAperiodic(g); }},
    {3, [](Automata g) { return isNotAperiodic(g); }},
    {3, [](Automata g) { return isSynchronized(g); }}
  };
  // Available predicate functions


  for (const auto& choice : choices) {
    if (choice > 0 && choice < 5) {
      this->predicates.push_back(allPredicates[choice]);
    } else {
      std::cout << "Invalid filter:" << choice << ". Ignored!" << std::endl;
    }
  }
};

template <typename GraphType>
bool SimpleFilter<GraphType>::isAccepted(GraphType& graph) {
  for (auto filter : this->predicates) {
    if (!filter(graph)) return false;
  }
  return true;
}

template class SimpleFilter<DirectedGraph>;
template class SimpleFilter<Automata>;