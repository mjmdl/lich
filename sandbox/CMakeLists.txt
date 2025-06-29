cmake_minimum_required(VERSION 3.22)

project(sandbox
	DESCRIPTION "Lich Sandbox"
	VERSION "0.0.1"
	LANGUAGES CXX)

add_executable(sandbox
	source/events_logger_layer.cpp
	source/game.cpp
	source/main.cpp
	source/render_layer.cpp)

target_include_directories(sandbox PRIVATE source)

target_link_libraries(sandbox PRIVATE lich)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(sandbox PRIVATE -Wall -Wextra -Wpedantic -g)
elseif(MSVC)
	target_compile_options(sandbox PRIVATE /W4)
endif()
