cmake_minimum_required(VERSION 3.10)
project(DA2026_PROJ2_T06_G04)

set(CMAKE_CXX_STANDARD 20)

add_executable(DA2026_PROJ2_T06_G04 main.cpp
        menu.cpp
        menu.h
        parser.cpp
        parser.h
        output_file.cpp
        output_file.h
        web_creation.cpp
        web_creation.h)
