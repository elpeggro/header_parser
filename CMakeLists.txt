cmake_minimum_required(VERSION 3.9)
project(header_parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS_RELEASE}")
set(HEADER_PARSER_SRC_FILES
        src/header_parse
        src/structs.h
        src/helper_functions
        src/defines.h)
add_executable(header_parser ${HEADER_PARSER_SRC_FILES})