cmake_minimum_required(VERSION 3.22)

project(lich
	DESCRIPTION "Lich Engine"
	VERSION "0.0.1"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/binaries)

add_library(lich STATIC
	source/lich/app.cpp
	source/lich/glfw_input.cpp
	source/lich/glfw_window.cpp
	source/lich/imgui.cpp
	source/lich/layer.cpp
	source/lich/log.cpp
	source/lich/opengl_renderer.cpp)

target_include_directories(lich
	PUBLIC source
	PRIVATE source/lich)

target_precompile_headers(lich PUBLIC source/lich/pch.hpp)

target_compile_definitions(lich PRIVATE LICH_COMPILE_STEP=1)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
	target_compile_options(lich PRIVATE -Wall -Wextra -Wpedantic)
elseif(MSVC)
	target_compile_options(lich PRIVATE /W4)
endif()

add_subdirectory(vendor)
target_link_libraries(lich PUBLIC
	expected
	glad
	glfw
	glm
	imgui
	spdlog
	stb)

add_subdirectory(sandbox)
