cmake_minimum_required(VERSION 3.22)

project(sandbox
	DESCRIPTION "Lich Sandbox"
	VERSION "0.0.1"
	LANGUAGES CXX)

add_executable(sandbox
	source/game.cpp
	source/main.cpp)

target_include_directories(sandbox PRIVATE source)

target_link_libraries(sandbox PRIVATE lich)
