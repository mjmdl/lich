cmake_minimum_required(VERSION 3.22)

project(
	stb
		DESCRIPTION "stb single-file public domain libraries for C/C++"
		VERSION "2.3.0"
		LANGUAGES C
)

add_library(
	stb STATIC
		source/stb_image.c
)

target_include_directories(
	stb
		PRIVATE include/stb
		PUBLIC include
)
