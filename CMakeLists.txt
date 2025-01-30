cmake_minimum_required(VERSION 3.29)
project(PEA_projekt_3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(PEA_projekt_3 main.cpp
        App.cpp
        Helpers.cpp
        Model.cpp
        algorithms/Greedy.cpp
        algorithms/SimAnnealing.cpp
        algorithms/TabuSearch.cpp
        Menu.cpp
        algorithms/GeneticAlgorithm.cpp
        algorithms/GeneticAlgorithm.h)
