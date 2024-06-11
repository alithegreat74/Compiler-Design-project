cmake_minimum_required(VERSION 3.5.0)

project(Compiler)


file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.cpp")

list(APPEND SRC_FILES "${CMAKE_SOURCE_DIR}/main.cpp")

add_executable(Compiler ${SRC_FILES})

target_include_directories(Compiler PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src"
                          )

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/test.c" "${CMAKE_CURRENT_BINARY_DIR}/src/test.c")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Compiler Tokens.txt" "${CMAKE_CURRENT_BINARY_DIR}/src/Compiler Tokens.txt")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Grammar.txt" "${CMAKE_CURRENT_BINARY_DIR}/src/Grammar.txt")