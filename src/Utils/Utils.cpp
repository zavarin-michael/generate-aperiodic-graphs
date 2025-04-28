#include "Utils.h"

#include <sstream>

#include "iostream"
#include <string>


std::vector<int> getInput() {
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
