cmake_minimum_required(VERSION 3.28)
project(PEAProjekt4)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEAProjekt4
        Main.cpp
        Main.hpp
        TSP.hpp
        TSP.cpp
        File_manager.hpp
        File_manager.cpp)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
set(CMAKE_EXE_LINKER_FLAGS "-static -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -std=c++11")
