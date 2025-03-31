cmake_minimum_required(VERSION 3.30)
project(generate_aperiodic_graphs)

find_package(Boost REQUIRED)
set(CMAKE_CXX_STANDARD 20)

add_executable(
        generate_aperiodic_graphs
        src/generate-graphs.cpp
        src/Recorders/DiskRecorder/DiskRecorder.tpp
        src/Recorders/ConsoleRecorder/ConsoleRecorder.cpp
        src/Recorders/NoRecorder/NoRecorder.cpp
        src/Generators/AllTwoOutgoingEdges/AllTwoOutgoingEdges.cpp
        src/Generators/RandomTwoOutgoingEdges/RandomTwoOutgoingEdges.cpp
        src/Filters/SimpleFilter/SimpleFilter.cpp
        src/Filters/Predicates/Predicates.cpp
        src/Readers/IReader.h
        src/analyze-automatas.cpp
)
add_executable(
        generate_automatas
        src/generate-automatas.cpp
        src/Readers/SingleGraphReader/SingleGraphReader.cpp
        src/Recorders/ConsoleRecorder/ConsoleRecorder.cpp
)
target_link_libraries(generate_aperiodic_graphs boost::boost)
target_link_libraries(generate_automatas boost::boost)
include_directories(${CMAKE_SOURCE_DIR}/src)
