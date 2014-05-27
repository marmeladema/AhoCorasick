cmake_minimum_required(VERSION 2.6)
project(AhoCorasick)

include_directories("include")
include_directories("src")

set(CMAKE_CPP_FLAGS "-g -Wall")
set(CMAKE_C_FLAGS "-g -Wall -Wextra")

add_library(ahocorasick SHARED
	include/ahocorasick.h
	src/ahocorasick.c
)

add_library(ahocorasick_static STATIC
	include/ahocorasick.h
	src/ahocorasick.c
)

add_executable(test_original
	tests/test_original.c
)
target_link_libraries(test_original ahocorasick)

add_executable(test_wikipedia
	tests/test_wikipedia.c
)
target_link_libraries(test_wikipedia ahocorasick)
