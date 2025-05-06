#include "Utils.h"

#include <chrono>
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


void printProgress(const std::string& title,  size_t i, size_t graphs_count,
                    const std::chrono::high_resolution_clock::time_point& start) {
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);

    auto div = static_cast<double>(i) / static_cast<double>(graphs_count);
    double estimated_total_time = (double) duration.count() / div;
    auto estimated_remaining_time = static_cast<int>(estimated_total_time * (1.0 - div));

    std::cout << title << "\n";
    std::cout << "Time to end: "
              << estimated_remaining_time / 3600 << "h "
              << (estimated_remaining_time % 3600) / 60 << "m "
              << estimated_remaining_time % 60 << "s\n";

    std::cout << "Graphs calculated: " << i << "\n";
    std::cout << "Percent of all graphs calculated: " << div * 100 << "%\n";
    std::cout << "------------------------\n";
}
