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
        src/Generators/Functions/Functions.cpp
)
add_executable(
        generate_automatas
        src/generate-automatas.cpp
        src/Readers/SingleGraphReader/SingleGraphReader.cpp
        src/Recorders/ConsoleRecorder/ConsoleRecorder.cpp
        src/Generators/Functions/Functions.cpp
)
add_executable(
        analyze_automatas
        src/analyze-automatas.cpp
        src/Readers/SingleGraphReader/SingleGraphReader.cpp
        src/Filters/SimpleFilter/SimpleFilter.cpp
        src/Filters/Predicates/Predicates.cpp
        src/Generators/Functions/Functions.cpp
        src/Recorders/DiskRecorder/DiskRecorder.tpp
        src/Recorders/ConsoleRecorder/ConsoleRecorder.cpp
        src/Recorders/NoRecorder/NoRecorder.cpp
)
target_link_libraries(generate_aperiodic_graphs boost::boost)
target_link_libraries(generate_automatas boost::boost)
target_link_libraries(analyze_automatas boost::boost)
include_directories(${CMAKE_SOURCE_DIR}/src)
