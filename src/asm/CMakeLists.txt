project(asm)

set(asm_SOURCES
	lexer.cpp
	token.cpp)

add_library(${PROJECT_NAME} ${asm_SOURCES})

