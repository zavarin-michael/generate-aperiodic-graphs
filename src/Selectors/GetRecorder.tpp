#include <iostream>
#include <Recorders/ConsoleRecorder/ConsoleRecorder.h>
#include <Recorders/DiskRecorder/DiskRecorder.h>
#include <Recorders/NoRecorder/NoRecorder.h>
#include <types/types.h>

template<typename GraphType>
IRecorder<GraphType>* getRecorder(const std::function<IRecorder<GraphType>*()>& getDefaultRecorder, const std::string& defaultRecorderName) {
    std::cout << "Select a Recorder [" << defaultRecorderName << "]" << std::endl;
    std::cout << "1: DiskRecorder" << std::endl;
    std::cout << "2: ConsoleRecorder" << std::endl;
    std::cout << "3: NoRecorder" << std::endl;

    std::string option;
    std::getline(std::cin, option);

    if (option.empty()) {
        return getDefaultRecorder();
    }

    int choice = 0;
    try {
        choice = std::stoi(option);
    } catch (...) {
        choice = -1; // Invalid input
    }

    switch (choice) {
        case 1:
            return new DiskRecorder<GraphType>();
        case 2:
            return new ConsoleRecorder<GraphType>();
        case 3:
            return new NoRecorder<GraphType>();
        default:
            std::cout << "Invalid option. Defaulting to NoRecorder" << std::endl;
        return new NoRecorder<GraphType>();
    }
}

template IRecorder<Graph>* getRecorder(const std::function<IRecorder<Graph>*()>& getDefaultRecorder, const std::string& defaultRecorderName);
template IRecorder<Automata>* getRecorder(const std::function<IRecorder<Automata>*()>& getDefaultRecorder, const std::string& defaultRecorderName);