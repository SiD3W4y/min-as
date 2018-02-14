project(codegen)

set(codegen_SOURCES
	instruction.cpp
    builder.cpp)

add_library(${PROJECT_NAME} ${codegen_SOURCES})

