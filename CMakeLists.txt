cmake_minimum_required(VERSION 3.9)
project(header_parser)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall ${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 ${CMAKE_CXX_FLAGS_RELEASE}")
find_package(LibXml2 REQUIRED)
add_definitions(${LIBXML2_DEFINITIONS})
set(HEADER_PARSER_SRC_FILES
        src/header_parse
        src/structs.h
        src/helper_functions
        src/defines.h
        src/XmlHandler)
add_executable(header_parser ${HEADER_PARSER_SRC_FILES})
target_include_directories(header_parser PRIVATE ${LIBXML2_INCLUDE_DIRS} /usr/include/libxml2)
target_link_libraries(header_parser ${LIBXML2_LIBRARIES})
