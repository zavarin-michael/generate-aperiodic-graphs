#include <chrono>
#include <string>
#include "vector"

#ifndef UTILS_H
#define UTILS_H


std::vector<int> getInput();
std::string getVertexName(long long c);
std::string computeIntLabelFromNodeId(const std::string& node_id);
void printProgress(const std::string& title, size_t i, size_t graphs_count, const std::chrono::high_resolution_clock::time_point& start);



#endif
