cmake_minimum_required(VERSION 3.10)
project(DA2026_PROJ2_T06_G04)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA2026_PROJ2_T06_G04 src/main.cpp
        src/menu.cpp
        src/menu.h
        src/parser.cpp
        src/parser.h
        src/output_file.cpp
        src/output_file.h
        src/webProcessing.cpp
        src/webProcessing.h
        src/dataStruct.h
        src/createGraph.cpp
        src/createGraph.h
        src/graph.h
        src/regAlloc.cpp
        src/regAlloc.h)
