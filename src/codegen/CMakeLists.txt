project(codegen)

set(codegen_SOURCES
	instruction.cpp
    builder.cpp
    ops.cpp)

add_library(${PROJECT_NAME} ${codegen_SOURCES})

