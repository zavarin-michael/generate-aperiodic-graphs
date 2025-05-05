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

std::string getVertexName(long long c) {
    return 'v' + std::to_string(c);
}

std::string computeIntLabelFromNodeId(const std::string& node_id) {
    if (!node_id.empty() && node_id[0] == 'v') {
        return node_id.substr(1);
    }
    return node_id;
}
