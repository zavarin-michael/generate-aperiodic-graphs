#include <iostream>
#include <Generators/AllTwoOutgoingEdges/AllTwoOutgoingEdges.h>
#include <Generators/RandomTwoOutgoingEdges/RandomTwoOutgoingEdges.h>
#include <types/types.h>

template<>
IGenerator<Graph>* getGenerator(const std::function<IGenerator<Graph>*()>& getDefaultGenerator, const std::string& defaultGeneratorName) {
    std::cout << "Select a Generator [" << defaultGeneratorName << "]" << std::endl;
    std::cout << "1: AllTwoOutgoingEdges" << std::endl;
    std::cout << "2: RandomTwoOutgoingEdges" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    if (option.empty()) {
        return getDefaultGenerator();
    }

    int choice = 0;
    try {
        choice = std::stoi(option);
    } catch (...) {
        choice = -1; // Invalid input
    }

    switch (choice) {
        case 1:
            return new AllTwoOutgoingEdges<Graph>();
        case 2:
            return new RandomTwoOutgoingEdges<Graph>();
        default:
            std::cout << "Invalid option. Defaulting to NoRecorder" << std::endl;
        throw;
    }
}

template IGenerator<Graph>* getGenerator(const std::function<IGenerator<Graph>*()>& getDefaultGenerator, const std::string& defaultGeneratorName);
// template IGenerator<Automata>* getGenerator(const std::function<IGenerator<Automata>*()>& getDefaultGenerator, const std::string& defaultGeneratorName);
