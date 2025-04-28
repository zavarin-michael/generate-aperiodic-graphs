#include "AutomatasFromGraph.h"

#include <iostream>
#include <boost/xpressive/regex_primitives.hpp>
#include <utility>

#include "Utils/Utils.h"

template<>
AutomatasFromGraph<AutomataGenerationResult>::AutomatasFromGraph(DirectedGraph& g, bool userInput, bool isHalf, std::vector<int> masks) : graph(g) {
    this->isHalf = isHalf;

    if (userInput) {
        std::string input;
        std::cout << "\n"
                  << "+=====================================+\n"
                  << "|        GENERATOR INITIALIZATION     |\n"
                  << "+=====================================+\n\n";

        std::cout << std::left << "-> isHalf [" << (isHalf ? "true" : "false") << "]: ";
        std::getline(std::cin, input);
        if (!input.empty()) {
            this->isHalf = !(input == "false");
        } else {
            this->isHalf = false;
        }

        std::cout << std::left << "-> Specific automatas [all]: ";
        const auto choices = getInput();
        if (!choices.empty()) {
            this->masks = choices;
        } else {
            this->masks = {};
        }
    } else {
        this->isHalf = isHalf;
        this->masks = masks;
    }
}

template<typename Type>
void AutomatasFromGraph<Type>::generateAutomata(long long mask, Automata& automata) {
    std::map<DirectedGraph::vertex_descriptor, Automata::vertex_descriptor> vertex_map;

    // vertexes copy
    for (auto [vi, vi_end] = boost::vertices(graph); vi != vi_end; ++vi) {
        auto v_new = boost::add_vertex(automata);
        vertex_map[*vi] = v_new;
    }

    unsigned int marked = 0;
    for (auto [ei, ei_end] = boost::edges(graph); ei != ei_end; ++ei) {
        auto src = boost::source(*ei, graph);
        auto tgt = boost::target(*ei, graph);

        auto new_edge = boost::add_edge(vertex_map[src], vertex_map[tgt], automata).first;
        automata[new_edge].mark = (mask >> src ^ marked >> src) & 1 ? 'b' : 'a';
        marked |= 1 << src;
    }

    auto [vi, vi_end] = boost::vertices(automata);
    for (auto it = vi; it != vi_end; ++it) {
        automata[*it].node_id = std::to_string(*it);
    }
}

template<>
AutomataPairCoroutine::pull_type AutomatasFromGraph<AutomataGenerationResult>::generateGraphs() {
    return AutomataPairCoroutine::pull_type([this](AutomataPairCoroutine::push_type &yield) {
        const auto maxMask = 1 << (isHalf ? graph.m_vertices.size() - 1 : graph.m_vertices.size());

        if (!masks.empty()) {
            for (auto mask : masks) {
                auto copy = std::make_shared<Automata>();
                auto &labeled = *copy;

                generateAutomata(mask, labeled);

                AutomataGenerationResult result;
                result.mask = mask;
                result.automata = copy;

                yield(result);
            }
        } else {
            for (long long mask = 0; mask < maxMask; mask++) {
                auto copy = std::make_shared<Automata>();
                auto &labeled = *copy;

                generateAutomata(mask, labeled);

                AutomataGenerationResult result;
                result.mask = mask;
                result.automata = copy;

                yield(result);
            }
        }
    });
}

template class AutomatasFromGraph<AutomataGenerationResult>;
