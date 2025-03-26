cmake_minimum_required(VERSION 3.30)
project(generate_aperiodic_graphs)

find_package(Boost REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(generate_aperiodic_graphs
        src/main.cpp
        src/Recorders/DiskRecorder/DiskRecorder.cpp
        src/Recorders/ConsoleRecorder/ConsoleRecorder.cpp
        src/Generators/TwoOutgoingEdges/TwoOutgoingEdges.cpp
        src/Filters/SimpleFilter/SimpleFilter.cpp
        src/Filters/Predicates/Predicates.cpp
)
target_link_libraries(generate_aperiodic_graphs boost::boost)
include_directories(${CMAKE_SOURCE_DIR}/src)
