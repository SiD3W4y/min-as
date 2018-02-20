project(min_as)

cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

add_subdirectory(src/utils)
add_subdirectory(src/asm)
add_subdirectory(src/codegen)

add_executable(min-as src/main.cpp)

target_link_libraries(min-as asm codegen utils)
