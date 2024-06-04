cmake_minimum_required(VERSION 3.0.0)

project(Compiler)

add_executable(Compiler main.cpp)

add_subdirectory(src)

target_link_libraries(Compiler Lexical)
target_link_libraries(Compiler Syntaxical)

target_include_directories(Compiler PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/src"
                          )

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/test.c" "${CMAKE_CURRENT_BINARY_DIR}/src/test.c")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Compiler Tokens.txt" "${CMAKE_CURRENT_BINARY_DIR}/src/Compiler Tokens.txt")
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/src/Grammar.txt" "${CMAKE_CURRENT_BINARY_DIR}/src/Grammar.txt")