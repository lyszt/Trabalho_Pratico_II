cmake_minimum_required(VERSION 4.0)
project(Trabalho_Pratico_II C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Trabalho_Pratico_II main.c
        sorting.c
        sorting.h)
