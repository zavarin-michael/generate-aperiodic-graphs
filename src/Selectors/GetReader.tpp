#include <iostream>
#include <Readers/IReader.h>
#include <Readers/SingleGraphReader/SingleGraphReader.h>
#include "Readers/MultipleGraphsReaders/MultipleGraphsReader.h"
#include <types/types.h>


template<typename GraphType>
IReader<GraphType>* getReader(const std::function<IReader<GraphType>*()>& getDefaultReader, const std::string& defaultReaderName) {
    std::cout << "GraphType: " << TypeName<GraphType>::get() << "\n";
    std::cout << "Select a Reader [" << defaultReaderName << "]" << std::endl;
    std::cout << "1: SingleGraphReader" << std::endl;
    std::cout << "2: MultipleGraphsReader" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    if (option.empty()) {
        return getDefaultReader();
    }

    int choice = 0;
    try {
        choice = std::stoi(option);
    } catch (...) {
        choice = -1; // Invalid input
    }

    switch (choice) {
        case 1:
            return new SingleGraphReader<GraphType>();
        case 2:
            return new MultipleGraphsReader<GraphType>();
        default:
            std::cout << "Invalid option. Defaulting to SingleGraphReader" << std::endl;
        return new SingleGraphReader<GraphType>();
    }
}

template IReader<DirectedGraph>* getReader(const std::function<IReader<DirectedGraph>*()>& getDefaultReader, const std::string& defaultReaderName);
template IReader<Automata>* getReader(const std::function<IReader<Automata>*()>& getDefaultReader, const std::string& defaultReaderName);


