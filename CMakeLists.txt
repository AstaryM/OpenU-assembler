cmake_minimum_required(VERSION 3.21)
project(untitled1 C)

set(CMAKE_C_STANDARD 90)
set(CMAKE_CXX_FLAGS_DEBUG 1)

add_executable(untitled1 main.c utils.h symbol_table.h second_pass.h pre_processor.h macro_table.h machine_word.h line_processing.h first_pass.h definitions.h utils.c symbol_table.c pre_proccesor.c macro_table.c machine_word.c line_processing.c first_pass.c second_pass.c output_files.c output_files.h
        temp.cpp)
