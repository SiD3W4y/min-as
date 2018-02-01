project(min_as)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

include_directories(include)

add_subdirectory(src/asm)

add_executable(min-as src/main.cpp)
target_link_libraries(min-as asm)
